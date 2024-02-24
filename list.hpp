#ifndef LIST_H
#define LIST_H


class Elem {
public:
    int index;
    int num;
    Elem *next, *prev;

    Elem() : index(0), num(0), next(nullptr), prev(nullptr) {}
};

class List {
public:
    Elem *first, *last, *cur;

    int size;

    List() : first(nullptr), last(nullptr), cur(nullptr), size(0) {}

    ~List() { delList(); }

    void add(int i);

    void add_head(int i);

    void del_head();

    void del_last();

    void printList();

    void printListPrev();

    void delList();

    int get_size() const;

    List(const List &other);

    List &operator=(const List &other);
};

void print_List(List *&sp, const std::string &name);

List *sum_list(List *sp, List *sp1);

#endif
