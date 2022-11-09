#ifndef VUTF8_VALIDATOR_H
#define VUTF8_VALIDATOR_H

#include <string>

//=======================================================================================
class vutf8_validator final
{
public:
    using cstring = const std::string&;

    static bool valid( cstring test_str, int *err_pos = nullptr ) noexcept;

    //  Works as conveer
    void append( cstring new_portion ) noexcept;
    void append( unsigned char ch    ) noexcept;

    bool error()    const noexcept; //  Error detected, use position() for find.
    bool between()  const noexcept; //  Between codepoints.

    int  position() const noexcept; //  Position is text or detected error position.

private:
    enum Stage
    {
        Between,
        Error,
        Wait_3_Suffix,
        Wait_2_Suffix,
        Wait_1_Suffix,
    };
    Stage stage = Between;
    int pos = 0;
};
//=======================================================================================

#endif // VUTF8_VALIDATOR_H
