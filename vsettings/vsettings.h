#ifndef VSETTINGS_H
#define VSETTINGS_H

#include <memory>
#include <vector>
#include <ostream>
#include "vcat.h"

//=======================================================================================
class vsettings final
{
public:
    using str        = std::string;
    using cstr       = const std::string&;
    using str_vector = std::vector<str>;

    class schema;

    void set( cstr key, cstr val, cstr comment = {} );
    str  get( cstr key ) const;

    template<typename T> T    get( cstr key ) const;
    template<typename T> void set( cstr key, const T& val, cstr comment = {} );

    vsettings& subgroup( cstr name, cstr comment = {} );
    const vsettings& subgroup( cstr name ) const;

    bool has_key      ( cstr key  ) const;
    bool has_subgroup ( cstr name ) const;

    bool del_key      ( cstr key  );
    bool del_subgroup ( cstr name );

    str_vector keys()      const;
    str_vector subgroups() const;

    str to_ini() const;

    //  NB! Не очищает свое содержимое! Читает значения поверх того, что уже есть.
    //  Можно прочитать два и более ini и они объединяться, для одинаковых ключей
    //  выиграет последнее прочитанное значение.
    void from_ini( cstr ini );

    void from_ini_file( cstr fname );
    void to_ini_file  ( cstr fname ) const;

    vsettings();
    ~vsettings();

    cstr comment_of_key      ( cstr key  ) const;
    cstr comment_of_subgroup ( cstr name ) const;

    static bool is_valid_key      ( cstr key  );
    static bool is_valid_subgroup ( cstr name );

    vsettings(const vsettings&) = default;
    vsettings& operator =(const vsettings&) = default;

private:
    class _pimpl; std::shared_ptr<_pimpl> _p;
    template<typename T> struct _getter;
};
//=======================================================================================
class vsettings::schema final
{
public:
    template<typename T>
    void add( cstr key, T *val, cstr comment = {} );

    void capture( const vsettings& settings );
    void capture_from_ini( cstr fname );

    vsettings build() const;
    void save_to_ini( cstr fname ) const;

    void subgroup( cstr name, cstr comment = {} );
    void end_subgroup();

private:
    //  Развязка для аккуратного хранения типизированных указателей на данные.
    struct _node_iface;
    using  _node_ptr = std::shared_ptr<_node_iface>;
    template <typename T> struct _node_iface2;
    template <typename T> struct _node;
    void _add_node( _node_ptr && );

    std::vector<_node_ptr> _nodes;

    struct _group_t
    {
        using vector = std::vector<_group_t>;
        str name, comment;
    };
    _group_t::vector _groups;
    friend bool operator == ( const _group_t::vector& lhs, const _group_t::vector& rhs );
};
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
std::ostream& operator << (std::ostream& os, const vsettings& sett );
//=======================================================================================
template<typename T>
struct vsettings::_getter
{
    static T get( cstr val )
    {
        return vcat::from_text<T>( val );
    }
};
//---------------------------------------------------------------------------------------
template<>
struct vsettings::_getter<bool>
{
    static bool get( cstr val )
    {
        std::string low;
        for ( auto ch: val )
            low.push_back( std::tolower(ch) );

        if      ( low == "true"  ) return true;
        else if ( low == "on"    ) return true;
        else if ( low == "false" ) return false;
        else if ( low == "off"   ) return false;
        else throw std::runtime_error( vcat("Cannot interpret bool value '",val,"'") );
    }
};
//---------------------------------------------------------------------------------------
template<>
struct vsettings::_getter<std::string>
{
    static str get( cstr val )
    {
        return val;
    }
};
//---------------------------------------------------------------------------------------
template<typename T>
T vsettings::get( cstr key ) const
{
    return _getter<T>::get( get(key) );
}
//=======================================================================================
template<typename T>
void vsettings::set( cstr key, const T& val, cstr comment )
{
    set( key, vcat().max_precision()(val).str(), comment );
}
//=======================================================================================


//=======================================================================================
struct vsettings::schema::_node_iface
{
    str key, comment;
    _group_t::vector groups;

    _node_iface( cstr k, cstr c )
        : key     ( k )
        , comment ( c )
    {}

    virtual void* stored_ptr() const                = 0;

    virtual void load( const vsettings& settings )  = 0;
    virtual void save( vsettings* settings ) const  = 0;

    virtual ~_node_iface();
};
//=======================================================================================
template <typename T>
struct vsettings::schema::_node_iface2 : vsettings::schema::_node_iface
{
    //-----------------------------------------------------------------------------------
    T* ptr;
    //-----------------------------------------------------------------------------------
    _node_iface2( cstr k, cstr c, T *p )
        : _node_iface( k, c )
        , ptr( p )
    {}
    //-----------------------------------------------------------------------------------
    virtual void* stored_ptr() const override
    {
        return ptr;
    }
    //-----------------------------------------------------------------------------------
    void load( const vsettings& settings ) override
    {
        const vsettings *sett_ptr = &settings;

        for ( auto & g: groups )
            sett_ptr = &sett_ptr->subgroup( g.name );

        *ptr = sett_ptr->get<T>( key );
    }
    //-----------------------------------------------------------------------------------
    void save( vsettings* settings ) const override
    {
        for ( auto& g: groups )
            settings = &settings->subgroup( g.name, g.comment );

        settings->set( key, *ptr, comment );
    }
    //-----------------------------------------------------------------------------------
};
//=======================================================================================
template <typename T>
struct vsettings::schema::_node : vsettings::schema::_node_iface2<T>
{
    //-----------------------------------------------------------------------------------
    _node( cstr k, cstr c, T * p )
        : vsettings::schema::_node_iface2<T>( k, c, p )
    {}
    //-----------------------------------------------------------------------------------
};
//=======================================================================================
template <>
struct vsettings::schema::_node<bool> : vsettings::schema::_node_iface2<bool>
{
    //-----------------------------------------------------------------------------------
    _node( cstr k, cstr c, bool * p )
        : vsettings::schema::_node_iface2<bool>( k, c, p )
    {}
    //-----------------------------------------------------------------------------------
    void load( const vsettings& settings ) override
    {
        const vsettings *sett_ptr = &settings;

        for ( auto & g: groups )
            sett_ptr = &sett_ptr->subgroup( g.name );

        auto text = sett_ptr->get( key );
        std::string low;
        for ( auto ch: text )
            low.push_back( std::tolower(ch) );

        if      ( low == "true"  ) *ptr = true;
        else if ( low == "on"    ) *ptr = true;
        else if ( low == "false" ) *ptr = false;
        else if ( low == "off"   ) *ptr = false;
        else throw std::runtime_error( vcat("Cannot interpret bool value '",text,"'") );
    }
    //-----------------------------------------------------------------------------------
};
//=======================================================================================
template <>
struct vsettings::schema::_node<std::string> : vsettings::schema::_node_iface2<str>
{
    //-----------------------------------------------------------------------------------
    _node( cstr k, cstr c, std::string * p )
        : vsettings::schema::_node_iface2<std::string>( k, c, p )
    {}
    //-----------------------------------------------------------------------------------
    void load( const vsettings& settings ) override
    {
        const vsettings *sett_ptr = &settings;

        for ( auto & g: groups )
            sett_ptr = &sett_ptr->subgroup( g.name );

        *ptr = sett_ptr->get( key );
    }
    //-----------------------------------------------------------------------------------
};
//=======================================================================================
template<typename T>
void vsettings::schema::add( cstr key, T *val, cstr comment )
{
    auto ptr = std::make_shared<_node<T>>(key,comment,val);
    _add_node( ptr );
}
//=======================================================================================


#endif // VSETTINGS_H
