#ifndef IMPL_VPOSIX_WRAP_TERMIOS_H
#define IMPL_VPOSIX_WRAP_TERMIOS_H


namespace impl_vposix
{

    class wrap_termios
    {
    public:
        //  Using TIO**** and ioctl, so if will create wrap_ioctl -- need move it there.
        static void set_tio_exclusive( int fd, bool excl );

        static void flush( int fd );
    };

} // namespace impl_vposix

#endif // IMPL_VPOSIX_WRAP_TERMIOS_H
