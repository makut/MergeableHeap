# include <set>
# ifndef IHEAP
# define IHEAP
# include "IHeap.h"
# endif // IHEAP

class SillyHeap : public IHeap
{
private:
    std::multiset<int> heap;
public:
    explicit SillyHeap(const int &key)
    {
        heap.insert(key);
    }

    SillyHeap(){}

    void Insert(const int &key)
    {
        heap.insert(key);
    }

    bool IsEmpty() const
    {
        return heap.empty();
    }

    int GetMin() const
    {
        return *heap.begin();
    }

    void ExtractMin()
    {
        heap.erase(heap.begin());
    }

    void Meld(IHeap *Iother)
    {
        SillyHeap *other = dynamic_cast<SillyHeap*>(Iother);
        heap.insert(other->heap.begin(), other->heap.end());
    }
};
