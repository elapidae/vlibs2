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

#include "vcompiler_traits.h"

//=======================================================================================
class vbyte_buffer_view;
//=======================================================================================
class vbyte_buffer
{
public:
    //-----------------------------------------------------------------------------------

    using vector = std::vector<vbyte_buffer>;
    using uint   = unsigned int;        //  For MinGW
    using ulong  = unsigned long;       //  compatibility

    //-----------------------------------------------------------------------------------

    vbyte_buffer();
    vbyte_buffer( std::string seed );
    vbyte_buffer( const char* seed );

    // Возврат буфера в виде строки
    const std::string& str() const noexcept;

    // Неявное преобразование vbyte_buffer к std::string
    operator const std::string&() const noexcept;

    size_t  size()  const noexcept;
    bool    empty() const noexcept;

    //  Прямой доступ к памяти буффера, для удобства, но будьте осторожны.
    const char*    data()  const noexcept;
    const int8_t*  sdata() const noexcept;
    const uint8_t* udata() const noexcept;

    //  NB! Ни в коем случае не изменяйте буффер пока пользуетесь view!
    vbyte_buffer_view view() const;

    // Очистка буфера
    void clear();

    bool operator == ( const vbyte_buffer& rhs ) const noexcept;
    bool operator != ( const vbyte_buffer& rhs ) const noexcept;

    vbyte_buffer& operator += ( const vbyte_buffer& rhs );

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

    // Возвратить count байт справа
    vbyte_buffer right  ( size_t count ) const;

    // Возвратить count байт, начиная с позиции буфера pos
    vbyte_buffer middle ( size_t pos, size_t count = std::string::npos ) const;

    //-----------------------------------------------------------------------------------

    // Проверка, начинается ли буфер данной строкой
    bool starts_with ( const std::string& what ) const;

    // Проверка, оканчивается ли буфер данной строкой
    bool ends_with   ( const std::string& what ) const;

    //-----------------------------------------------------------------------------------

    template <typename T> T to_any() const;

    // Набор методов, преобразующих строку буфера в необходимый формат

    int         to_int()       const   { return to_any<int>();        }
    uint        to_uint()      const   { return to_any<uint>();       }
    long        to_long()      const   { return to_any<long>();       }
    ulong       to_ulong()     const   { return to_any<ulong>();      }
    float       to_float()     const   { return to_any<float>();      }
    double      to_double()    const   { return to_any<double>();     }

    int8_t      to_i8()        const   { return to_any<int8_t>();     }
    uint8_t     to_u8()        const   { return to_any<uint8_t>();    }

    int16_t     to_i16()       const   { return to_any<int16_t>();    }
    uint16_t    to_u16()       const   { return to_any<uint16_t>();   }

    int32_t     to_i32()       const   { return to_any<int32_t>();    }
    uint32_t    to_u32()       const   { return to_any<uint32_t>();   }

    int64_t     to_i64()       const   { return to_any<int64_t>();    }
    uint64_t    to_u64()       const   { return to_any<uint64_t>();   }

    //-----------------------------------------------------------------------------------

    // Нечувствительна к регистру, все символы, кроме набора hex игнорируются.
    // NB! При нечетном количестве годных символов, считается, что первый -- ноль.
    static  vbyte_buffer from_hex(  const std::string& src );
            vbyte_buffer from_hex() const;

    vbyte_buffer tohex () const;                         // сплошным текстом, строчными.
    vbyte_buffer toHex () const;                         // сплошным текстом, Заглавными.
    vbyte_buffer to_hex( char separator = ' ' ) const;   // с разделителями, строчными.
    vbyte_buffer to_Hex( char separator = ' ' ) const;   // с разделителями, Заглавными.

    static bool is_hex_symbol( char ch ) noexcept;

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
    reverse_T( T src ) noexcept;

    //-----------------------------------------------------------------------------------

private:
    std::string _buf;

    template<typename T> void _append( const T& val );
}; // vbyte_buffer
//=======================================================================================
std::ostream& operator << ( std::ostream& os, const vbyte_buffer& buf );

vbyte_buffer operator + ( const vbyte_buffer& lhs, const vbyte_buffer& rhs );
//=======================================================================================


//=======================================================================================
//      Implememntation
//=======================================================================================
//  NB! Этот метод скопипащен из vcat::from_text.
//  Пока что, волевым усилием принято держать этот код и там и там.
template <typename T>
T vbyte_buffer::to_any() const
{
    std::istringstream ss( _buf );
    T res;
    ss >> res;

    if ( ss.fail() || ss.bad() )
        throw std::runtime_error( std::string("Error during interpretation text: '") +
                _buf + "', in function " + V_PRETTY_FUNCTION );

    return res;
}
//=======================================================================================

// deprecated
template<typename T>
typename std::enable_if< std::is_arithmetic<T>::value, T>::type
vbyte_buffer::reverse_T( T val ) noexcept
{
    return v::endian::reverse_val( val );
}
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
    val = v::endian::to_little_endian( val );
    _append( val );
}
//=======================================================================================
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
vbyte_buffer::append_BE( T val )
{
    val = v::endian::to_big_endian( val );
    _append( val );
}
//=======================================================================================

#endif // VBYTE_BUFFER_H
