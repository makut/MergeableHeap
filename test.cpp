# include <iostream>
# include <vector>
# include <algorithm>
# include <ctime>
# include "gtest/gtest.h"
# include "BHeap.h"
# include "BaseHeap.h"
# include "SillyHeap.h"

template<typename Heap>
class HeapList
{
private:
    std::vector<IHeap*> heaps;
public:
    bool empty() const
    {
        return heaps.empty();
    }

    size_t size() const
    {
        return heaps.size();
    }

    bool IsEmpty(const size_t &index) const
    {
        return heaps[index]->IsEmpty();
    }

    void AddHeap(const int &key)
    {
        heaps.push_back(dynamic_cast<IHeap*>(new Heap(key)));
    }

    void Insert(const size_t &index, const int &key)
    {
        heaps[index]->Insert(key);
    }

    int GetMin(const size_t &index) const
    {
        return heaps[index]->GetMin();
    }

    void ExtractMin(const size_t &index)
    {
        heaps[index]->ExtractMin();
    }

    void Meld(const size_t &index1, const size_t &index2)
    {
        if (index2 == index1) return;
        heaps[index1]->Meld(heaps[index2]);
        heaps.erase(heaps.begin() + index2);
    }
};

enum Operation
{
    ADDHEAP = 0,
    INSERT = 1,
    GETMIN = 2,
    EXTRACTMIN = 3,
    MELD = 4
};

template<typename Heap>
void TestTemplate()
{
    const size_t OPERATIONS = 100000;
    HeapList<SillyHeap> slist;
    HeapList<Heap> list;
    for (size_t i = 0; i < OPERATIONS; ++i)
    {
        EXPECT_EQ(list.size(), slist.size());
        EXPECT_EQ(list.empty(), slist.empty());
        if (list.size() == 0)
        {
            int key = rand();
            list.AddHeap(key);
            slist.AddHeap(key);
        }
        else
        {
            Operation op = static_cast<Operation>(rand() % 5);
            size_t index = rand() % slist.size();
            int key = rand();
            switch(op)
            {
            case ADDHEAP:
                list.AddHeap(key);
                slist.AddHeap(key);
            case INSERT:
                list.Insert(index, key);
                slist.Insert(index, key);
            case GETMIN:
            {
                bool empty1 = slist.IsEmpty(index);
                bool empty2 = list.IsEmpty(index);
                EXPECT_EQ(empty1, empty2);
                if (!empty1)
                {
                    int min1 = slist.GetMin(index);
                    int min2 = list.GetMin(index);
                    EXPECT_EQ(min1, min2);
                }
            }
            case EXTRACTMIN:
            {
                bool empty1 = slist.IsEmpty(index);
                bool empty2 = list.IsEmpty(index);
                EXPECT_EQ(empty1, empty2);
                if (!empty1)
                {
                    list.ExtractMin(index);
                    slist.ExtractMin(index);
                }
            }
            case MELD:
                size_t index1 = rand() % list.size();
                list.Meld(index, index1);
                slist.Meld(index, index1);
            }
        }
    }
}

TEST(BinomialHeap, Operations)
{
    std::cout << 1 << "\n";
    TestTemplate<BHeap>();
}

TEST(LeftishHeap, Operations)
{
    std::cout << 2 << "\n";
    TestTemplate<LHeap>();
}

TEST(SkewHeap, Operations)
{
    std::cout << 3 << "\n";
    TestTemplate<SHeap>();
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
