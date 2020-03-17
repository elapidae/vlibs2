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
    using string    = std::string;
    using cstring   = const std::string&;
    using str_list  = std::vector<string>;

    class schema;

    void set( cstring key, cstring val );

    string get( cstring key ) const;

    template<typename T>
    T get( cstring key ) const;

    template<typename T>
    void set( cstring key, const T& val );

    vsettings& subgroup( cstring name );
    const vsettings& subgroup( cstring name ) const;

    bool has( cstring key ) const;
    bool has_subgroup(cstring name ) const;

    str_list keys() const;
    str_list subgroup_names() const;

    string str() const;
    void load( cstring data );

    void from_ini_file( cstring fname );
    void to_ini_file  ( cstring fname ) const;

    vsettings();
    ~vsettings();


    static bool is_valid_key      ( cstring key );
    static bool is_valid_subgroup ( cstring key );

    vsettings(const vsettings&) = default;
    vsettings& operator =(const vsettings&) = default;

private:
    class _pimpl; std::shared_ptr<_pimpl> _p;
};
//=======================================================================================
class vsettings::schema
{
public:
    template<typename T>
    void add( cstring key, T *val );

    void capture( const vsettings& settings );
    void capture_from_ini( cstring fname );

    vsettings build() const;
    void save_to_ini( cstring fname ) const;

    void subgroup( cstring name );
    void unsubgroup();

private:
    struct _node_iface;
    using  _node_ptr = std::shared_ptr<_node_iface>;
    template <typename T> struct _node;

    void _add_node( _node_ptr && );

    std::vector<_node_ptr> _nodes;
    str_list _groups;
};
//=======================================================================================


//=======================================================================================
std::ostream& operator << (std::ostream& os, const vsettings& sett );
//=======================================================================================
template<typename T>
T vsettings::get( cstring key ) const
{
    return vcat::from_text<T>( get(key) );
}
//=======================================================================================
template<typename T>
void vsettings::set( cstring key, const T& val )
{
    set( key, vcat(val).str() );
}
//=======================================================================================


//=======================================================================================
struct vsettings::schema::_node_iface
{
    std::string key;
    str_list groups;

    _node_iface( cstring k )
        : key( k )
    {}

    virtual void* mine() const                      = 0;
    virtual bool  same( void *check_ptr ) const     = 0;

    virtual void load( const vsettings& settings )  = 0;
    virtual void save( vsettings* settings ) const  = 0;

    virtual ~_node_iface();
};
//=======================================================================================
template <typename T>
struct vsettings::schema::_node : vsettings::schema::_node_iface
{
    //-----------------------------------------------------------------------------------
    T* ptr;
    //-----------------------------------------------------------------------------------
    _node( cstring k, T *p )
        : _node_iface( k )
        , ptr( p )
    {}
    //-----------------------------------------------------------------------------------
    virtual void* mine() const override
    {
        return ptr;
    }
    //-----------------------------------------------------------------------------------
    bool same( void *check_ptr ) const override
    {
        return ptr == check_ptr;
    }
    //-----------------------------------------------------------------------------------
    void load( const vsettings& settings ) override
    {
        const vsettings *sett_ptr = &settings;

        for ( auto& g: groups )
            sett_ptr = &sett_ptr->subgroup( g );

        *ptr = sett_ptr->get<T>( key );
    }
    //-----------------------------------------------------------------------------------
    void save( vsettings* settings ) const override
    {
        for ( auto& g: groups )
            settings = &settings->subgroup( g );

        settings->set(key, *ptr);
    }
    //-----------------------------------------------------------------------------------
};
//=======================================================================================
template<typename T>
void vsettings::schema::add( cstring key, T *val )
{
    auto ptr = std::make_shared<_node<T>>(key,val);
    _add_node( ptr );
}
//=======================================================================================


#endif // VSETTINGS_H
