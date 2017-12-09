# ifndef IHEAP
# define IHEAP
# include "IHeap.h"
# endif // IHEAP

class BHeap : public IHeap
{
private:
    struct BinomialTree
    {
        int key, size;
        BinomialTree *sibling;
        BinomialTree *largestChild;

        BinomialTree(const int &key_) : key(key_), size(1), sibling(NULL), largestChild(NULL) {}
        ~BinomialTree()
        {
            if (sibling != NULL) delete sibling;
            if (largestChild != NULL) delete largestChild;
        }

        void Meld(BinomialTree *other, BinomialTree *next)
        {
            size *= 2;
            sibling = next;
            other->sibling = largestChild;
            largestChild = other;
        }
    };

    BinomialTree *firstNode;
public:
    BHeap() : firstNode(NULL) {}

    explicit BHeap(const int &key)
    {
        firstNode = new BinomialTree(key);
    }

    BHeap(BinomialTree *firstNode) : firstNode(firstNode) {}

    ~BHeap()
    {
        delete firstNode;
    }

    bool IsEmpty() const
    {
        return (firstNode == NULL);
    }

    void Meld(IHeap *Iother)
    {
        BHeap *other = dynamic_cast<BHeap*>(Iother);
        if (other->firstNode == NULL) return;
        BinomialTree *answer;
        BinomialTree *first = firstNode, *second = other->firstNode;
        if (firstNode == NULL)
        {
            answer = second;
            second = second->sibling;
        }
        else
        {
            if (first->size <= second->size)
            {
                answer = first;
                first = first->sibling;
            }
            else
            {
                answer = second;
                second = second->sibling;
            }
        }
        BinomialTree *current = answer;
        while (first != NULL && second != NULL)
        {
            if (first->size <= second->size)
            {
                current->sibling = first;
                current = current->sibling;
                first = first->sibling;
            }
            else
            {
                current->sibling = second;
                current = current->sibling;
                second = second->sibling;
            }
        }
        if (first == NULL)
        {
            while (second != NULL)
            {
                current->sibling = second;
                current = current->sibling;
                second = second->sibling;
            }
        }
        else
        {
            while (first != NULL)
            {
                current->sibling = first;
                current = current->sibling;
                first = first->sibling;
            }
        }
        current = answer;
        BinomialTree *previous = NULL;
        while (current->sibling != NULL)
        {
            BinomialTree *next = current->sibling;
            if (current->size == next->size)
            {
                if (current->key <= next->key)
                {
                    current->Meld(next, next->sibling);
                }
                else
                {
                    next->Meld(current, next->sibling);
                    if (previous != NULL) previous->sibling = next;
                    else answer = next;
                    current = next;
                }
            }
            else
            {
                previous = current;
                current = current->sibling;
            }
        }
        firstNode = answer;
    }

    void Insert(const int &key)
    {
        Meld(dynamic_cast<IHeap*>(new BHeap(key)));
    }

    int GetMin() const
    {
        int answer = firstNode->key;
        BinomialTree *current = firstNode;
        while (current != NULL)
        {
            answer = std::min(answer, current->key);
            current = current->sibling;
        }
        return answer;
    }

    void ExtractMin()
    {
        int minimum = GetMin();
        BinomialTree *current = firstNode, *previous = NULL;
        while (current->key != minimum)
        {
            previous = current;
            current = current->sibling;
        }
        if (previous != NULL) previous->sibling = current->sibling;
        else
        {
            firstNode = current->sibling;
        }
        current->sibling = NULL;
        if (current->largestChild == NULL)
        {
            delete current;
            return;
        }
        BinomialTree *previousChild = NULL, *currentChild = current->largestChild;
        while (currentChild != NULL)
        {
            BinomialTree *temporal = currentChild->sibling;
            currentChild->sibling = previousChild;
            previousChild = currentChild;
            currentChild = temporal;
        }
        current->largestChild = NULL;
        delete current;
        BHeap *newHeap = new BHeap(previousChild);
        Meld(newHeap);
    }
};
