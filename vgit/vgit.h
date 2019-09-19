/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/

#ifndef VGIT_H
#define VGIT_H

#include <string>
#include <tuple>

//=======================================================================================
class vgit
{
public:
    static std::string hash();
    static std::string revcount();
    static std::string branch();
    static std::string author();
    static std::string date();

    // Метка времени компиляции.
    // К git отношения не имеет, но здесь вполне уместно...
    static std::string compile_datetime();

    // принимает --vgit-branch, --vgit-hash, --vgit-revcount, --vgit-date, vgit-author,
    //           --vgit-full.
    // Возвращает, соответственно. По умолчанию и при ошибках использует --vgit-full
    static std::string as_message( const std::string &query = "--vgit-full" );

    // ищет параметры вида --vgit*, если найдет, печатает соответствующее сообщение
    // и выходит по exit(retcode);
    static void print_and_exit_if_need( int argc,
                                        const char * const * const argv,
                                        int retcode = 0 );

    class entry
    {
    public:
        std::string hash;
        std::string revcount;
        std::string branch;
        std::string author;
        std::string date;
    };

    static entry cur_entry();
};
//=======================================================================================


//=======================================================================================
//  Чтобы два раза не вставать, сразу сделаем сериализацию.
namespace s11n
{
    template<typename T> struct Serial;

    template<>
    struct Serial< vgit::entry >
    {
        using _s = std::string;

        static std::tuple<_s,_s,_s,_s,_s>
        to_tuple( const vgit::entry& e )
        {
            return  std::make_tuple( e.hash,
                                     e.revcount,
                                     e.branch,
                                     e.author,
                                     e.date );
        }
    };
} // s11n namespace
//=======================================================================================

#endif // VGIT_H
