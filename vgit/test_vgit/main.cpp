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

// TODO!
//  Использован linux-овский popen/pclose для чтения подпроцессов. Не особо грамотно,
//  хотелось бы разобраться, чтобы были грамотные обертки для работы с процессами.

#include "gtest/gtest.h"
#include <iostream>
#include "vgit.h"

using namespace std;

static std::string last_fname(const char *filepath)
{
    std::string fp(filepath);
    auto pos = fp.find_last_of( '/' );
    if ( pos == std::string::npos ) return fp;
    return fp.substr( pos + 1 );
}

#define vdeb std::cout << last_fname(__FILE__) << ":" << __LINE__ << "==> "

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VGit_Test: public testing::Test
{};
#pragma GCC diagnostic pop


//=======================================================================================

TEST_F( VGit_Test, hash )
{
    FILE* f = popen( "git log -n 1 --pretty=format:\"%H\"", "r" );

    char buf[41];
    auto sz = fread(buf, 1, 41, f);
    EXPECT_EQ(sz, 40);  // size of SHA hash.

    EXPECT_EQ(vgit::hash(), string(buf,sz) );

    pclose(f);
    vdeb << "vgit::hash(): " << vgit::hash() << endl;
}

//=======================================================================================

TEST_F( VGit_Test, revcount )
{
    FILE* f = popen( "git rev-list HEAD --count", "r" );

    enum { BufSZ = 100 };
    char buf[BufSZ];
    auto sz = fread(buf, 1, BufSZ, f);

    // Читаем в интовую переменную,
    istringstream is( string(buf,sz) );
    int count; is >> count;
    EXPECT_FALSE( is.fail() ); // проверяем, что
    EXPECT_FALSE( is.bad()  ); // поток не накрылся.

    // Пишем в еще один поток.
    ostringstream os; os << count;

    // Проверяем правильно ли взяли.
    EXPECT_EQ( os.str(), vgit::revcount() );

    pclose( f );
    vdeb << "vgit::revcount(): " << vgit::revcount() << endl;
}

//=======================================================================================

TEST_F( VGit_Test, branch )
{
    FILE* f = popen( "git symbolic-ref --short HEAD", "r" );

    enum { BufSZ = 100 };
    char buf[BufSZ];
    auto sz = fread(buf, 1, BufSZ, f);
    auto pres = string(buf, sz);

    if ( !pres.empty() && pres.back() == '\n' ) pres.pop_back();
    if ( !pres.empty() && pres.back() == '\r' ) pres.pop_back();

    // есть такоой глюююююкккк....
    if ( vgit::branch() == "DETACHED HEAD" )
    {
        vdeb << "detached head detected..." << endl;
        EXPECT_TRUE( pres.empty() );
        pres = "DETACHED HEAD";
    }

    EXPECT_EQ( pres, vgit::branch() );

    pclose( f );
    vdeb << "vgit::branch(): " << vgit::branch() << endl;
}

//=======================================================================================

TEST_F( VGit_Test, autor_name )
{
    FILE* f = popen( "git log -n 1 --pretty=format:\"%an\"", "r" );

    enum { BufSZ = 1000 };
    char buf[BufSZ];
    auto sz = fread(buf, 1, BufSZ, f);
    auto pres = string(buf, sz);

    // Проверяем правильно ли взяли.
    EXPECT_EQ( pres, vgit::author_name() );

    pclose( f );
    vdeb << "vgit::author_name(): " << vgit::author_name() << endl;

}

//=======================================================================================

TEST_F( VGit_Test, autor_email )
{
    FILE* f = popen( "git log -n 1 --pretty=format:\"%ae\"", "r" );

    enum { BufSZ = 1000 };
    char buf[BufSZ];
    auto sz = fread(buf, 1, BufSZ, f);
    auto pres = string(buf, sz);

    // Проверяем правильно ли взяли.
    EXPECT_EQ( pres, vgit::author_email() );

    pclose( f );
    vdeb << "vgit::author_email(): " << vgit::author_email() << endl;

}

//=======================================================================================

TEST_F( VGit_Test, date )
{
    FILE* f = popen( "git log -n 1 --pretty=format:\"%ci\"", "r" );

    enum { BufSZ = 100 };
    char buf[BufSZ];
    auto sz = fread(buf, 1, BufSZ, f);
    auto pres = string(buf, sz);

    // Проверяем правильно ли взяли.
    EXPECT_EQ( pres, vgit::date() );

    pclose( f );
    vdeb << "vgit::date(): " << vgit::date() << endl;
}

//=======================================================================================

TEST_F( VGit_Test, entry )
{
    auto entry = vgit::cur_entry();
    EXPECT_EQ( entry.hash,          vgit::hash()            );
    EXPECT_EQ( entry.revcount,      vgit::revcount()        );
    EXPECT_EQ( entry.branch,        vgit::branch()          );
    EXPECT_EQ( entry.author_name,   vgit::author_name()     );
    EXPECT_EQ( entry.author_email,  vgit::author_email()    );
    EXPECT_EQ( entry.date,          vgit::date()            );
    EXPECT_EQ( entry.vlibs,         vgit::vlibs_msg()       );
}

//=======================================================================================

TEST_F( VGit_Test, test_vlibs_fix )
{
    EXPECT_EQ( vgit::hash(),        vgit::vlibs_hash()      );
    EXPECT_EQ( vgit::revcount(),    vgit::vlibs_revcount()  );
}

//=======================================================================================

TEST_F( VGit_Test, test_macro )
{
    bool ok =
        #if VLIBS_REVCOUNT >= 235
            true;
        #else
            false;
        #endif

    EXPECT_TRUE( ok );
}

//=======================================================================================

TEST_F( VGit_Test, test_vlibs_feature )
{
    vdeb << "We can know vlibs ver: " << vgit::vlibs_msg() << endl;
    vdeb << "And commits count (use macro VLIBS_REVCOUNT):" << VLIBS_REVCOUNT << endl;
}

//=======================================================================================


//=======================================================================================
//  Main, do not delete...
//=======================================================================================
int main(int argc, char *argv[])
{
    vdeb << "Cur revcount + 1: " << (VLIBS_REVCOUNT + 1) << "\n\n";

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
//  Main, do not delete...
//=======================================================================================
