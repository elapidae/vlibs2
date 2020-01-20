#ifndef VBYTE_BUFFER_H
#define VBYTE_BUFFER_H

//=======================================================================================
/*  vbyte_buffer            2019-11-22  by elapidae
 *
 *  БАЙТОВЫЙ БУФФЕР ДЛЯ СОСТАВЛЕНИЯ И ЧТЕНИЯ БИНАРНЫХ ДАННЫХ, но с текстовыми плюшками.
 *
 *  Читайте API!
 *
 *  У класса нету задачи предоставить всеобъемлющий интерфейс для работы со строками
 *  (урок с прошлого раза, VString работает хорошо, но тяжелая).
 *
 *  Главная задача: составление данных в бинарных протоколах, но, чтобы два раза не
 *  вставать, сделана группа методов для обработки текстовых данных.
*/
//=======================================================================================

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

//=======================================================================================
class vbyte_buffer_view;
//=======================================================================================
class vbyte_buffer
{
public:
    //-----------------------------------------------------------------------------------

    using vector = std::vector<vbyte_buffer>;

    //-----------------------------------------------------------------------------------

    explicit vbyte_buffer();
    explicit vbyte_buffer( std::string seed );

    // Возврат буфера в виде строки
    const std::string& str() const;

    // Неявное преобразование vbyte_buffer к std::string
    operator const std::string&() const;

    size_t size() const;

    //  NB! Ни в коем случае не изменяйте буффер пока пользуетесь view!
    vbyte_buffer_view view() const;

    // Очистка буфера
    void clear();

    bool operator == ( const vbyte_buffer& rhs ) const;
    bool operator != ( const vbyte_buffer& rhs ) const;

    //-----------------------------------------------------------------------------------

    //  Группа основных методов: append
    //  Append Little Endian
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, void>::type
    append_LE( T val );

    //  Append Big Endian
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, void>::type
    append_BE( T val );

    void append( unsigned char val ); //  однобайтовые могут быть добавлены как есть.
    void append( signed char   val ); //
    void append( char          val ); //

    void append( const std::string& val );  //  строки и их аналоги
    void append( const char* val );         //

    //-----------------------------------------------------------------------------------

    //  Отрезать count байт с разных сторон.
    //  Исключения не бросаются, если размер будет больше, буфер очистится.
    void chop_front ( size_t count );
    void chop_back  ( size_t count );

    //  NB! Если count будет больше текущего размера, бросится
    //  исключение std::length_error, потому что нечем заполнять.
    void resize( size_t count );

    // Возвратить count байт слева
    vbyte_buffer left   ( size_t count ) const;

    // Возвратить count байт слева
    vbyte_buffer right  ( size_t count ) const;

    // Возвратить count байт, начиная с позиции буфера pos
    vbyte_buffer middle ( size_t pos, size_t count = std::string::npos ) const;

    //-----------------------------------------------------------------------------------

    // Проверка, начинается ли буфер данной строкой
    bool starts_with ( const std::string& what ) const;

    // Проверка, оканчивается ли буфер данной строкой
    bool ends_with   ( const std::string& what ) const;

    //-----------------------------------------------------------------------------------

    template <typename T> T buf_to_any() const;

    // Набор методов, преобразующих строку буфера в необходимый формат

    int         buf_to_int()       const   { return buf_to_any<int>();        }
    uint        buf_to_uint()      const   { return buf_to_any<uint>();       }
    long        buf_to_long()      const   { return buf_to_any<long>();       }
    ulong       buf_to_ulong()     const   { return buf_to_any<ulong>();      }
    float       buf_to_float()     const   { return buf_to_any<float>();      }
    double      buf_to_double()    const   { return buf_to_any<double>();     }

    int8_t      buf_to_i8()        const   { return buf_to_any<int8_t>();     }
    uint8_t     buf_to_u8()        const   { return buf_to_any<uint8_t>();    }

    int16_t     buf_to_i16()       const   { return buf_to_any<int16_t>();    }
    uint16_t    buf_to_u16()       const   { return buf_to_any<uint16_t>();   }

    int32_t     buf_to_i32()       const   { return buf_to_any<int32_t>();    }
    uint32_t    buf_to_u32()       const   { return buf_to_any<uint32_t>();   }

    int64_t     buf_to_i64()       const   { return buf_to_any<int64_t>();    }
    uint64_t    buf_to_u64()       const   { return buf_to_any<uint64_t>();   }

    //-----------------------------------------------------------------------------------

    // Нечувствительна к регистру, все символы, кроме набора hex игнорируются.
    // NB! При нечетном количестве годных символов, считается, что первый -- ноль.
    static  vbyte_buffer from_hex(  const std::string& src );
            vbyte_buffer from_hex() const;

    vbyte_buffer tohex () const;                         // сплошным текстом, строчными.
    vbyte_buffer toHex () const;                         // сплошным текстом, Заглавными.
    vbyte_buffer to_hex( char separator = ' ' ) const;   // с разделителями, строчными.
    vbyte_buffer to_Hex( char separator = ' ' ) const;   // с разделителями, Заглавными.

    static bool is_hex_symbol( char ch );

    //-----------------------------------------------------------------------------------
    //  NB! Методы split не добавляют в конец пустое значение, если разделитель был
    //  последним символом. Т.е. split("A|B", '|') == split("A|B|", '|')
    //  А вот для пустой входной строки будет возвращен пустой вектор:
    //      split("", '|') == vector{}, но split("|", '|') == vector{vbyte_buffer{}};

    //  split() режет по splitter, пустые тоже захватывает.
    static vector split( const std::string& data, char splitter );

    vector split( char splitter ) const;

    //  Разрезает текст, используя разделители всех мастей (std::isspace).
    //  Выкидывает все пустые, если так режем, по любому, работаем с текстом.
    vector split_by_spaces() const;

    //  Отсекает начальные и конечные пробелы.
    vbyte_buffer& trim_spaces();

    //-----------------------------------------------------------------------------------

    // Выворачивает байты наизнанку, т.е. <LE> <-> <BE>.
    template<typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    reverse_T( T src );

    //-----------------------------------------------------------------------------------

private:
    std::string _buf;

    template<typename T> void _append( const T& val );
}; // vbyte_buffer
//=======================================================================================
std::ostream& operator << ( std::ostream& os, const vbyte_buffer& buf );
//=======================================================================================


//=======================================================================================
//      Implememntation
//=======================================================================================
//  NB! Этот метод скопипащен из vcat::from_text.
//  Пока что, волевым усилием принято держать этот код и там и там.
template <typename T>
T vbyte_buffer::buf_to_any() const
{
    std::istringstream ss( _buf );
    T res;
    ss >> res;

    if ( ss.fail() || ss.bad() )
        throw std::runtime_error( std::string("Error during interpretation text: '") +
                _buf + "', in function " + __PRETTY_FUNCTION__ );

    return res;
}
//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
template<typename T>
typename std::enable_if< std::is_arithmetic<T>::value, T>::type
vbyte_buffer::reverse_T( T val )
{
    static_assert ( sizeof(T) <= 8, "Cannot reverse values more than 8 bytes." );

    auto *ch = static_cast<char*>( static_cast<void*>(&val) );
    constexpr auto tsize = sizeof(T);

    switch( tsize )
    {
    case 8: std::swap( ch[3], ch[tsize-4] );
            std::swap( ch[2], ch[tsize-3] );
    case 4: std::swap( ch[1], ch[tsize-2] );
    case 2: std::swap( ch[0], ch[tsize-1] );
    }

    return val;
}
#pragma GCC diagnostic pop
//=======================================================================================
template<typename T>
void vbyte_buffer::_append( const T& val )
{
    const char* ptr = static_cast<const char*>( static_cast<const void*>(&val) );
    _buf.append( ptr, sizeof(T) );
}
//=======================================================================================
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
vbyte_buffer::append_LE( T val )
{
    #if BYTE_ORDER == BIG_ENDIAN
        val = vbyte_buffer::reverse_T( val );
    #endif
    _append( val );
}
//=======================================================================================
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
vbyte_buffer::append_BE( T val )
{
    #if BYTE_ORDER == LITTLE_ENDIAN
        val = vbyte_buffer::reverse_T( val );
    #endif
    _append( val );
}
//=======================================================================================

#endif // VBYTE_BUFFER_H
