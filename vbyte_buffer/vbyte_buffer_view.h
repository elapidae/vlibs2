#ifndef VBYTE_BUFFER_VIEW_H
#define VBYTE_BUFFER_VIEW_H

#include "vbyte_buffer.h"

//=======================================================================================
class vbyte_buffer_view
{
public:
    //-----------------------------------------------------------------------------------

    // Конструктор view, который смотрит на буфер buf длиной len
    vbyte_buffer_view( const char* buf,          size_t len )   noexcept;
    vbyte_buffer_view( const signed char* buf,   size_t len )   noexcept;
    vbyte_buffer_view( const unsigned char* buf, size_t len )   noexcept;

    //-----------------------------------------------------------------------------------

    //  Прямой доступ к памяти буффера, для удобства, но будьте осторожны.
    const char*    data()  const noexcept;
    const int8_t*  sdata() const noexcept;
    const uint8_t* udata() const noexcept;

    // Сколько элементов буффера осталось впереди
    size_t remained() const noexcept;

    // Закончился ли просматриваемый буфер
    bool   finished() const noexcept;

    // Пропустить некоторое количество
    void omit( size_t count );

    //-----------------------------------------------------------------------------------

    // Отобразить следующий элемент в формате Little Endian
    template<typename T> T show_LE()  const;

    // Отобразить следующий элемент в формате Big Endian
    template<typename T> T show_BE()  const;

    // Отобразить sz следующих элементов буффера в виде строки
    std::string  show_string ( size_t sz ) const;

    // Отобразить sz следующих элементов буффера
    vbyte_buffer show_buffer ( size_t sz ) const;

    // Просмотр оставшейся части
    vbyte_buffer show_tail() const;

    //-----------------------------------------------------------------------------------

    // Извлечь из буфера следующий элемент в формате Little Endian
    template<typename T> T LE();

    // Извлечь из буфера следующий элемент в формате Big Endian
    template<typename T> T BE();

    // Извлечь из буфера строку размера sz
    std::string  string ( size_t sz );

    // Извлечь из буфера буфер размера sz
    vbyte_buffer buffer ( size_t sz );

    // Извлечь оставшуюся часть
    vbyte_buffer tail();

    //-----------------------------------------------------------------------------------

    char     show_ch()        const         { return show_LE<char>();       }
    int8_t   show_i8()        const         { return show_LE<int8_t>();     }
    uint8_t  show_u8()        const         { return show_LE<uint8_t>();    }

    int16_t  show_i16_LE()    const         { return show_LE<int16_t>();    }
    int16_t  show_i16_BE()    const         { return show_BE<int16_t>();    }
    uint16_t show_u16_LE()    const         { return show_LE<uint16_t>();   }
    uint16_t show_u16_BE()    const         { return show_BE<uint16_t>();   }

    int32_t  show_i32_LE()    const         { return show_LE<int32_t>();    }
    int32_t  show_i32_BE()    const         { return show_BE<int32_t>();    }
    uint32_t show_u32_LE()    const         { return show_LE<uint32_t>();   }
    uint32_t show_u32_BE()    const         { return show_BE<uint32_t>();   }

    int64_t  show_i64_LE()    const         { return show_LE<int64_t>();    }
    int64_t  show_i64_BE()    const         { return show_BE<int64_t>();    }
    uint64_t show_u64_LE()    const         { return show_LE<uint64_t>();   }
    uint64_t show_u64_BE()    const         { return show_BE<uint64_t>();   }


    float    show_float_LE()  const         { return show_LE<float>();      }
    float    show_float_BE()  const         { return show_BE<float>();      }

    double   show_double_LE() const         { return show_LE<double>();     }
    double   show_double_BE() const         { return show_BE<double>();     }

    //-----------------------------------------------------------------------------------

    char     ch()                           { return LE<char>();            }
    int8_t   i8()                           { return LE<int8_t>();          }
    uint8_t  u8()                           { return LE<uint8_t>();         }

    int16_t  i16_LE()                       { return LE<int16_t>();         }
    int16_t  i16_BE()                       { return BE<int16_t>();         }
    uint16_t u16_LE()                       { return LE<uint16_t>();        }
    uint16_t u16_BE()                       { return BE<uint16_t>();        }

    int32_t  i32_LE()                       { return LE<int32_t>();         }
    int32_t  i32_BE()                       { return BE<int32_t>();         }
    uint32_t u32_LE()                       { return LE<uint32_t>();        }
    uint32_t u32_BE()                       { return BE<uint32_t>();        }

    int64_t  i64_LE()                       { return LE<int64_t>();         }
    int64_t  i64_BE()                       { return BE<int64_t>();         }
    uint64_t u64_LE()                       { return LE<uint64_t>();        }
    uint64_t u64_BE()                       { return BE<uint64_t>();        }

    float    float_LE()                     { return LE<float>();           }
    float    float_BE()                     { return BE<float>();           }

    double   double_LE()                    { return LE<double>();          }
    double   double_BE()                    { return BE<double>();          }

    //-----------------------------------------------------------------------------------

private:
    const char *_buffer;
    size_t      _remained;

    template<typename T> T _show() const;
    template<typename T> T _extract();
};
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
template<typename T>
T vbyte_buffer_view::_show() const
{
    if ( _remained < sizeof(T) )
        throw std::out_of_range( "vbyte_buffer_view: not enough data" );

    T res;
    auto ptr = static_cast<char*>( static_cast<void*>(&res) );
    std::copy( _buffer, _buffer + sizeof(T), ptr );

    return res;
}
//=======================================================================================
template<typename T>
T vbyte_buffer_view::_extract()
{
    auto res = _show<T>();

    _remained -= sizeof (T);
    _buffer   += sizeof (T);

    return res;
}
//=======================================================================================
template<typename T>
T vbyte_buffer_view::show_LE() const
{
    auto res = _show<T>();

    #if BYTE_ORDER == BIG_ENDIAN
        res = vbyte_buffer::reverse_T( res );
    #endif

    return res;
}
//=======================================================================================
template<typename T>
T vbyte_buffer_view::show_BE() const
{
    auto res = _show<T>();

    #if BYTE_ORDER == LITTLE_ENDIAN
        res = vbyte_buffer::reverse_T( res );
    #endif

    return res;
}
//=======================================================================================
template<typename T>
T vbyte_buffer_view::LE()
{
    auto res = _extract<T>();

    #if BYTE_ORDER == BIG_ENDIAN
        res = vbyte_buffer::reverse_T( res );
    #endif

    return res;
}
//=======================================================================================
template<typename T>
T vbyte_buffer_view::BE()
{
    auto res = _extract<T>();

    #if BYTE_ORDER == LITTLE_ENDIAN
        res = vbyte_buffer::reverse_T( res );
    #endif

    return res;
}
//=======================================================================================

#endif // VBYTE_BUFFER_VIEW_H
