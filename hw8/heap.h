#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
template <
         typename T,
         typename TComparator = std::equal_to<T>,
         typename PComparator = std::less<double>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const TComparator& tcomp = TComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item with the provided priority
    void push(double pri, const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey reduces the current priority of
    /// item to newpri, moving it up in the heap
    /// as appropriate.
    void decreaseKey(double newpri, const T& item);

private:
    /// Add whatever helper functions you need below
    void trickleDown(int pos); 
    void trickleUp(int pos);
   // void swap(int locf, int locr);

    // These should be all the data members you need.
    std::vector< std::pair<double, T> > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, TComparator> keyToLocation_;

};

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const TComparator& tcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, tcomp)

{

}

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::~Heap()
{

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::push(double priority, const T& item)
{
    // You complete.
    store_.push_back(std::make_pair(priority, item));
    keyToLocation_.insert(std::make_pair(item, store_.size()-1));
    trickleUp(store_.size()-1);

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::decreaseKey(double priority, const T& item)
{
    // You complete
    int index_ = keyToLocation_[item];
    store_[index_].first = priority;
   // keyToLocation_[item] = priority;
    trickleUp(index_);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T const & Heap<T,TComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    // You complete
    return store_[0].second;

}

/// Removes the top element
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    // You complete
    T item = store_[0].second;
    keyToLocation_.erase(item);
    store_[0] = store_.back(); 
    store_.pop_back();
    trickleDown(0); 

}

/// returns true if the heap is empty
template <typename T, typename TComparator, typename PComparator, typename Hasher >
bool Heap<T,TComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleUp(int pos){
  int parent = (pos-1)/m_;
  while(parent>=0 && c_(store_[pos].first, store_[parent].first)){
    std::swap(store_[parent], store_[pos]);
    // swap key to location 
    pos = parent; 
    parent = (pos-1) / m_; 
    keyToLocation_[store_[pos].second] = pos;
  }
  //update location
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleDown(int pos){
  if (pos * m_ +1 < (int) store_.size()){
    //int smallest = pos; 
    int smallest_pos = pos*m_+1;
// t_item[pos*d_child+i] < smallest
    for ( int i =1; i <= m_; i++){
      if (pos*m_+i < (int) store_.size()){
        // convert i into index
        if (c_(store_[pos*m_+i].first , store_[smallest_pos].first)){
          //std::swap(t_item[smallest],t_item[pos*d_child+i]);
          smallest_pos = pos*m_+i;
        }
      }    
    }

    if ( c_(store_[smallest_pos].first, store_[pos].first)){
      std::swap(store_[smallest_pos], store_[pos]); 
      trickleDown(smallest_pos);    
    }
  }
}

#endif

