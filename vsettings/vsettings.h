#ifndef VSETTINGS_H
#define VSETTINGS_H

#include <memory>
#include <vector>
#include <ostream>
#include "vbyte_buffer.h"

//=======================================================================================
class vsettings final
{
public:
    using str       = std::string;
    using cstr      = const std::string&;
    using str_list  = std::vector<str>;

    class schema;

    void set( cstr key, cstr val );

    str  get( cstr key ) const;

    template<typename T>
    T get_as( cstr key ) const;

    vsettings& subgroup( cstr name );
    const vsettings& subgroup( cstr name ) const;

    bool has( cstr key ) const;
    bool has_subgroup(cstr name ) const;

    str_list keys() const;
    str_list subgroup_names() const;

    str  save() const;
    void load( cstr data );

    void load_from_ini( cstr fname );
    void save_to_ini  ( cstr fname ) const;

    vsettings();
    ~vsettings();


    static bool is_valid_key      ( cstr key );
    static bool is_valid_subgroup ( cstr key );

    vsettings(const vsettings&) = default;
    vsettings& operator =(const vsettings&) = default;

private:
    class _pimpl; std::shared_ptr<_pimpl> _p;
};
//=======================================================================================
class vsettings::schema
{
public:
    void add( cstr key, str* val );

    template<typename T>
    void add( cstr key, T* val );

    void capture( vsettings settings );
    void capture_from_ini( cstr fname );

    vsettings build() const;
    void save_to_ini( cstr fname ) const;
};
//=======================================================================================


//=======================================================================================
std::ostream& operator << (std::ostream& os, const vsettings& sett );
//=======================================================================================
template<typename T>
T vsettings::get_as( cstr key ) const
{
    return vbyte_buffer( get(key) ).to_any<T>();
}
//=======================================================================================


#endif // VSETTINGS_H
