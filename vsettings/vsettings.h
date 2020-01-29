#ifndef VSETTINGS_H
#define VSETTINGS_H

#include <memory>
#include <vector>

class vsettings final
{
public:
    using str = std::string;

    class record
    {
    public:
        using list = std::vector<record>;
        str key, value, comment;
    };

    class tree
    {
    public:
        tree();
        virtual ~tree();

        using list = std::vector<tree>;

        str group;
        record::list records;

        list subtrees;

        tree * parent = nullptr;

    protected:
        void changed()
        {
            if (parent)
                parent->changed();
            else
                save();
        }
        virtual void save() {}
    };

    class ini_file : tree
    {
    public:
        vsettings::tree& tree();
    };

    ~vsettings();

private:
    class _pimpl; std::shared_ptr<_pimpl> _p;
};



#endif // VSETTINGS_H
