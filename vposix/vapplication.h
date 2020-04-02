#ifndef VAPPLICATION_H
#define VAPPLICATION_H

#include <vector>
#include <string>
#include "vinvoke_iface.h"

//=======================================================================================
/*
    bool take( cstr arg );

    std::vector<str> unused() const;

    str take_std( cstr key );

    //  Помечает использованными, но если не найдет -- возвращает default_val.
    //  Возвращает default_val
    str safe_std( cstr key, cstr default_val );

};

 *
 *
 * TODO: Сделать полноценный pid-locker, чтобы захватывать атомарно.
*/
//=======================================================================================


//=======================================================================================
class vapplication final : public vinvoke_iface
{
public:
    size_t tasks_count() const override;
    void   tasks_clear() override;

    static void poll();
    static void stop();

    class args_parser;

    static pid_t pid() noexcept;

private:
    void _invoke( task_type && f ) override;
};
//=======================================================================================
//  args_parser -- альтернативное видение как парсить аргументы командной строки.
//
//  Использованные обозначения:
//  take_*  -- методы помечают аргумент использованными,
//              бросают исключение если что-то пошло не так.
//  safe_* --  методы помечают аргумент использованными,
//              НЕ бросают исключений если что-то пошло не так, возвращают default_val.
//
//  *_starts_with -- имеются ввиду аргументы, начинающиеся на определенную преамбулу,
//                  например, if=somefile.
//                  Возвращают часть, уже отрезанную от преамбулы (somefile).
//  *_next -- имеются ввиду аргументы, идущие друг за другом (key value).
//
class vapplication::args_parser
{
public:
    using str  = std::string;
    using cstr = const str&;

    class error : public std::runtime_error { public: error(cstr msg); };

    //  Передаем сюда наши аргументы.
    args_parser( int argc, char const * const * const argv );

    str app_name() const;   //  Имя программы, argv[0] после последнего слеша.
    str app_path() const;   //  Путь к программе, argv[0] до последнего слеша.
    str full_app() const;   //  Полное имя программы, банально argv[0].

    //  Сырые методы, надеюсь вам не понадобятся.
    //  вернет -1 если нету.
    int  index( cstr arg )                  const;
    int  index_starts_with( cstr preambul ) const;
    uint count()                            const;
    cstr get_by_index ( int idx )           const;
    cstr mark_by_index( int idx );

    //  Оставшиеся невыбранными аргументы.
    //  Может быть полезно для диагностики неправильного ввода аргументов.
    std::vector<str> unused() const;

    //  Есть ли такой аргумент.
    bool has( cstr arg ) const;

    //  Есть ли аргумент, который начинается на preambul.
    bool has_starts_with( cstr preambul ) const;

    //  Возвращает если есть такой аргумент и он помечается использованным.
    //  Этот take исключений не бросает.
    bool take ( cstr arg );

    //  Возвращает "хвост" от аргумента, начинающегося на preambul.
    //  safe не бросает исключения, возвращает default_val
    //  Помечается использованным.
    str take_starts_with( cstr preambul );
    str safe_starts_with( cstr preambul, cstr default_val  );

    //  next -- имеется ввиду два аргумента, следующие друг за другом.
    //  Помечает использованными.
    //  safe не бросает исключения, возвращает default_val.
    cstr take_next( cstr key );
    cstr safe_next( cstr key, cstr default_val );


    //  В принципе, можно замутить разбор как в unix.
    //  На наших задачах такого подхода пока не требуется.
    //  Если будет необходимость -- стучите.
    void unix_way();    // Not implemented.

private:
    std::vector<str>  _args;
    std::vector<bool> _used;

    int _pos( cstr arg ) const;
    int _count() const;
};
//=======================================================================================

#endif // VAPPLICATION_H
