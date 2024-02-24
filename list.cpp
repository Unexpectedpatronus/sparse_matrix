#include <iostream>
#include <stdexcept>

#include "list.hpp"

List::List(const List &other) : size(other.size) {
    first = last = cur = nullptr;
    Elem *temp = other.first;
    while (temp) {
        add(temp->index);
        temp = temp->next;
    }
}

List &List::operator=(const List &other) {
    if (this != &other) {
        delList();
        first = last = cur = nullptr;
        Elem *temp = other.first;
        while (temp) {
            add(temp->index);
            temp = temp->next;
        }
    }
    return *this;
}

void List::add(int i) {
    cur = new Elem();
    cur->index = i;
    if (first) {
        last->next = cur;
        cur->prev = last;
        last = cur;
    } else {
        first = cur;
        last = cur;
    }
    size++;
}

void List::add_head(int i) {
    cur = new Elem();
    cur->index = i;
    if (first == nullptr) {
        first = cur;
        last = cur;
    } else {
        cur->next = first;
        first->prev = cur;
        first = cur;
    }
    size++;
}

void List::del_head() {
    if (!first) {
        throw std::out_of_range("List is empty");
    }
    cur = first;
    first = first->next;
    if (first)
        first->prev = nullptr;
    else
        last = nullptr;
    delete cur;
    size--;
}

void List::del_last() {
    if (!last) {
        throw std::out_of_range("List is empty");
    }
    cur = last;
    last = last->prev;
    if (last)
        last->next = nullptr;
    else
        first = nullptr;
    delete cur;
    size--;
}

void List::printList() {
    cur = first;
    while (cur) {
        std::cout << cur->index << " ";
        cur = cur->next;
    }
    std::cout << std::endl;
}

void List::printListPrev() {
    cur = last;
    while (cur) {
        std::cout << cur->index << " ";
        cur = cur->prev;
    }
    std::cout << std::endl;
}

void List::delList() {
    while (first) {
        del_head();
    }
}

int List::get_size() const {
    return size;
}

void print_List(List *&sp, const std::string &name) {
    sp->cur = sp->first;
    while (sp->cur) {
        std::cout << name << " size = " << sp->get_size() << " index = "
                  << sp->cur->index << " num = " << sp->cur->num << std::endl;
        sp->cur = sp->cur->next;
    }
    std::cout << std::endl;
}

List *sum_list(List *sp, List *sp1) {
    int k, n;
    List *sps;
    sps = new List();
    sp1->cur = sp1->first;
    sp->cur = sp->first;
    n = 0;
    sps->add(n);

    while (sp1->cur && sp->cur) {
        if (sp->cur->index == sp1->cur->index) {
            sps->cur->num = sp->cur->num + sp1->cur->num;
            sp1->cur = sp1->cur->next;
            sp->cur = sp->cur->next;

        } else if (sp->cur->index < sp1->cur->index) {
            sps->cur->num = sp->cur->num;
            sps->cur->index = sp->cur->index;
            sp->cur = sp->cur->next;

        } else {
            sps->cur->num = sp1->cur->num;
            sps->cur->index = sp1->cur->index;
            sp1->cur = sp1->cur->next;
        }
        n++;
        sps->add(n);
    }
    if (sp1->cur) {
        while (sp1->cur) {
            sps->cur->num = sp1->cur->num;
            sps->cur->index = sp1->cur->index;
            sp1->cur = sp1->cur->next;
            sps->cur = sps->cur->next;
            n++;
            sps->add(n);
        }
    } else if (sp->cur) {
        while (sp->cur) {
            sps->cur->num = sp->cur->num;
            sps->cur->index = sp->cur->index;
            sp->cur = sp->cur->next;
            sps->cur = sps->cur->next;
            n++;
            sps->add(n);
        }
    }
    sps->del_last();
    return sps;
}
