#ifndef VFILE_H
#define VFILE_H

#include <string>

//=======================================================================================
class vfile
{
public:
    static bool unlink_no_err( const std::string& filename );

    virtual ~vfile();

    void close();

    std::string filename() const;

protected:
    std::string _filename;
    int _fd = -1;
};
//=======================================================================================
class vfile_in : public vfile
{

};
//=======================================================================================
class vfile_out : public vfile
{
public:
    enum method { truncate, append };

    vfile_out( const std::string& fname, method = truncate );
    void write( const std::string& data );
};
//=======================================================================================


#endif // VFILE_H
