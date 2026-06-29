// C++ 14 features
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <vector>
#include <deque>
#include "complex/complex.hpp"
#include <list>
#include <forward_list>
#include <unordered_set>
#include <stack>
#include <queue>
#include <set>
#include <algorithm>
#include <functional>
#include <utility>


[[deprecated("This gets replaced with template add function ")]]
int add (int a, int b) {return a+b;}
template <typename T>
T pi = T(3.141592453356374L);

auto increment(int &a) { // auto & would make it work, btw
    a++;
    return a; 
}
 

decltype(auto) decrement(int &a) { // checksthe type of a that it returns
    a--;
    return a; 
}


TEST(LinearSearchTest, Basic) {
  auto x = 1'000'000;
  std::cout << "x = " << x << std::endl;
  auto y = 0b1111;
  std::cout << "y = " << y << std::endl;

  // Deprecated Keyword
  
  std::cout << "Add 1,5 = " << add(1,5) << std::endl;

  // Variable template
  std::cout.precision(std::numeric_limits<long double>::max_digits10);
  std::cout << pi<int> << std::endl;
  std::cout << pi<float> << std::endl;
  std::cout << pi<double> << std::endl;
  std::cout << pi<long double> << std::endl;

  // Generic Lambda
  auto add_gen_lambda = [] (auto x, auto y) {return x + y ; }; 
  std::cout << add_gen_lambda( std::string("C++ "), std::string("Lambdas") ) << std::endl;

  // Return type deduction
  // decltype(auto)
  // 
  int p = 10;
  // int &p_ref = increment(p);
  // error: non-const lvalue reference to type 'int' cannot bind to a temporary of type 'int'  --- int &p_ref = increment(p);
  const int &p_ref = increment(p);
  std::cout << "p_ref is : " << p_ref << std::endl;

  // const int& p_ref = increment(p);   // ✅ allowed
  // A const T & can bind to a temporary (or rvalue), extending its life time to reference

  int &x_ref = decrement(p);
  std::cout << "x_ref is : " << x_ref << std::endl;

  // const int x = 0
  // auto x1 = x; // x1 becomes int
  // decltype(ayto) x2 = x; // const int
  // similary for reftype &
  // volatile, ref, comnst---> auto


  // Copy and swap idiom

}

template <typename T>
T my_max(T x, T y){
  return (x>y) ? x : y;

}




TEST(Template, Basic) {
  std::cout << "Max amont 3 and 7: " << my_max<int>(3,7) << std::endl;
  Complex<int> c1(1,2);
  Complex<int> c2(5,6);
  Complex<int> res = c1 + c2;
  EXPECT_EQ(Complex(6,8), res);
  res.print();
  //std::cout << "Addition of complex : " << re

  int arr[] = {10,2,3,4,5,6,7,30};
  auto size = sizeof(arr)/sizeof(arr[0]);
  int min = arr_min<int, 10000>(arr, size);
  std::cout << "Min = " << min << std::endl;


  //Class Template Arguments Deduction (C++17 Onwards)
  Complex c3(1.1,2.1);
  Complex c4(21.2,32.2);
  Complex c5 = c3 + c4;
  c5.print();

  std::cout << "Factorial of 5 is: " << Factorial<5>::val << std::endl;
}

TEST(STL, Basic) {
  // Sequential containers
  // vector, dequeue, list, forward list, array
  #include <array>
  std::array<int, 6> arr = {1,2,3,4,5,6};
  decltype(auto) p = arr.front();
  p++;
  decltype(auto) q = arr.back();
  q++;
  std::cout << "Array is " << std::endl;
  for (auto &i:arr)
    std::cout << " " <<  i << " " ;

  std::cout << "\n Size of array " << arr.size() << std::endl;
  std::cout << "Array empty..? --> " << arr.empty() << std::endl;

  arr.fill(1);
}


class Node {
  int *arr;
  int size;

  public:
    Node(int size, int fill_val) : size(size){
      arr = new int[size];
      std::fill_n(arr, size, fill_val);
    }
    ~Node(){
        delete arr;
    }
    Node(const Node &other) : size(other.size), arr(new int[other.size]) {
        std::copy(other.arr, other.arr + size, arr);
    }

//   Node & operator = (const Node & other) {
//     if(this != &other){ // if a different object
//       // delete data : destroy the old data
//       delete arr;
//       size = other.size;
//       arr = new int[size]; // memory allocation, may throw error
//       std::copy(other.arr, other.arr + size, arr);
//     }
//     return *this; // enables chaining a=b=c
//   }


/* Copy and swap Idiom */
  Node & operator = (Node other) {
    swap(*this, other);
    return *this;
  }

  friend void swap(Node &first, Node &second){
    std::swap(first.size,second.size);
    std::swap(first.arr, second.arr);
  }

  void display(){
    for (int i=0; i<size; i++) {
        std::cout << "arr[i] : " << arr[i] << std::endl;
    }
  }
};


TEST(CopyAndSwapIdiom, Basic) {
  Node n1(10, 1);
  Node n2(10, 5);
  n1 = n2;
  n1.display();
  n2.display();

}


class LNode{
  int data;
  LNode *next;
public:
  LNode(int _data): data(_data), next(nullptr){}
  void setNext(LNode *_next){
    next = _next;
  }
  void setData(int _data){
    data = _data;
  }
  int getData() {return data;}
  LNode * getNext(){
    return next;
  }

};

class LinkedList{
  LNode *head; // head a raw pointer, NOT a container, range based loops only work if it has begin(), end()


public:
  LinkedList(): head(nullptr){}
  
  void push_front(int data) {
    LNode* node = new LNode(data);
    node->setNext(head);
    head = node;
  }


  // motivated from list<int> :: iterator --> define an iterator

  class iterator {
   LNode *cur;  
  public:
    iterator(LNode * p): cur(p) {}
    // *it
    // *it returns integer
    auto operator * () {return cur->getData();}

    // it++, it !=r.end()

    iterator & operator ++ () {
      cur = cur->getNext();
      return *this;
    }

    // it!=r.end()
    bool operator != (const iterator &other) {
      return (cur != other.cur);
    }

  };


  // list l1, l1.begin(), l1.end()
  // it.begin(), it.end()
  iterator begin(){return iterator(head);}
  iterator end(){return iterator(nullptr);}

  void display(){
    for (auto it:*this){
      std::cout << "Linked list" << it << std::endl;
    }
  }
  
};
TEST(STL2, Basic){
  #include <vector>
  // Delete elements in vector
  // O(n)
  std::vector<int> v{1,2,3,4,5,6,7,8,9,10};
  v.erase(std::find(v.begin(), v.end(), 5));

  for (auto a: v){
    std::cout << " vector [i] : " << a << "size " << v.size() <<std::endl;
    
  }

  // vector of vectors or 2D vector
  std::vector<std::vector<int>> vec = {

    {1,2,3},
    {2,5},
    {4,5,7,9}
  };

  for (auto x: vec){
    for (auto y:x)
        std::cout << "vector 2D" << y << std::endl;
  }


  // Dequeue
  
  std::deque<int> dq = {1,2,3,4,5};

  dq.push_front(100);
  dq.pop_back();
  std::cout << "Dq back " << dq.back() << std::endl;
  int del_element = 3;
  auto it = std::find(dq.begin(), dq.end(), del_element);
  if(it != dq.end())
    std::cout << "Found" << std::endl;
  else
    std::cout << "Not found" << std::endl;
  if(it != dq.end())
    dq.erase(std::find(dq.begin(), dq.end(), del_element));
  for (auto x: dq)
    std::cout << "Dequeue Size " << dq.size() << "element " << x <<std::endl;


  // List --> actually double linked list
  // std::list<>
  // pop_front, pop_back, rbegin()--> iterator for last element
  // clear()
  // traverse
  std::list<int> l1 = {1,2,3,4,5,6,7,8,9,10};
  for (auto it: l1)
    std::cout << "List " << l1.size() << "element" << it << std::endl;

  auto it1 = l1.begin();
  while(it1 != l1.end()){
    std::cout << "List Manual" << *it1 << std::endl;
    std::advance(it1,1); // advance the iterator by 1
  }

  LinkedList l2;
  l2.push_front(3);
  l2.push_front(2);
  l2.push_front(1);
  for (auto k:l2){
    std::cout << "Manual iterator linked list " << k << std::endl;
  }
  for (LinkedList::iterator it2 = l2.begin(); it2 != l2.end() ; ++it2 ){
    std::cout << "Lisnde list iterator" << *it2 << std::endl;
  }


  // deque -- random access like vector, list only bidirectional operator
  // deque - contiguous
  // 

  std::forward_list<int> fl = {1,2,3,4,5,6,7,8,9,10,11};
  fl.push_front(100);
  auto it3 = fl.begin();
  std::advance(it3,3);
  fl.erase_after(it3);
  for (auto k:fl)
    std::cout << "Forward List" << k << std::endl;
}


TEST(AssociativeContainer , Basic) {
    // Set Map MultiSet Multimap
    // Search insert delete in O(logn), sorted order, Balanced Trees
    // 
    std::set <int> s1 = {1,2,3,4,5,6,7,8,9,10};
    s1.insert(11);
    s1.erase(10);
    auto p = s1.find(12);
    if(p != s1.end())
      std::cout << "Set 12 Found" << std::endl;
    else
      std::cout << "Set 12 NOT Found " << std::endl;
    auto [l,r] = s1.equal_range(6);
    std::cout << "Left range" << *l << std::endl;
    std::cout << "Right range" << *r << std::endl;


    // Map
    std::map<int, std::string> m1 = {
      {1,"Set"},
      {2,"Map"},
      {3, "Multimap"},
      {4, "MultiSet"}
    };

    
    
    m1.insert({0,"Associative Container"});
    m1.erase(4);
    for(auto x: m1)
        std::cout << x.first << x.second << std::endl;

    auto val3 = m1.find(3);
    std::cout << "Key3 := " << val3->first << "Val3 := " << val3->second << std::endl;



    // Multiset
    // insert()
    // erase()
    // count()
    // find()

    std::multiset<int> m2 = {1,2,3,1,2,3,1,2,3,1,2,3,4,5,6,7,8,9,10};
    for(auto k:m2)
        std::cout << "Multiset : " << k << std::endl;

    auto p2 = m2.equal_range(3);
    std::cout << "Equal range first: " << *p2.first << std::endl;
    std::cout << "Equal range second:  " << *p2.second << std::endl;

    for (auto l=p2.first; l != p2.second; l++)
        std::cout << "l = " << *l << std::endl;

    std::multimap <int , std::string> m3 = {
      {1,"ANOVA"}, 
      {2, "Confidentiality Interval"}, 
      {3, "Hypotheses Testing"},
      {3, "Gaussian"}, 
      {2, "Linear Regression"}
    };

    auto find3 = m3.find(3);
    m3.erase(find3);
    for (auto q:m3)
        std::cout << q.first << q.second << std::endl;

    // Unordered associative containers
    // unordered set, map, multiset, multimap


    std::unordered_set<int> us1 = {1,2,3,4,5,6,7,8,9};
    std::cout << "Max bucket count " << us1.max_bucket_count() << std::endl;
    us1.insert(9999);
    std::cout << "Max bucket count " << us1.max_bucket_count() << std::endl;

    std::unordered_map<std::string , std::string> um = {
        {"India", "New Delhi"},
        {"Pakistan", "Islamabad"},
        {"Afghanistan", "Kabul"}
    };

    for (auto j:um)
        std::cout << j.first << " " <<  j.second << std::endl;

    std::cout << um["Afghanistan"] << std::endl;

    std::unordered_map<std::string, std::string> um2 = {
      {"Mazaar Sharif", "Afghanistan"},
      {"AnantNaag", "India"}, 
      {"Peshawar", "Pakistan"}, 
      {"Tehraan" , "Iran"}
    };

    um.merge(um2);
    for (auto x:um)
        std::cout << "After Merging " << x.first << " " << x.second << std::endl;

    // after merge um2 becomes null
    // if key collides - merge never overwrite, that key remains in um2


    std::unordered_multimap<int, std::string> umm = {
      {1, "Stack"},
      {2, "Queue"},
      {3, "Priority Queue"},
      {1, "Stack using array"}, 
      {2, "Queue using LinkedList"}
    };

    std::cout << "Count wioth key 2 :=  " << umm.count(2) << std::endl;
    umm.erase(2);
    for (auto p:umm)
        std::cout << p.first << " " << p.second << std::endl;

    std::stack<std::string> st;
    st.push("111");
    st.push("222");
    st.push("333");

    while(not st.empty()){
        std::cout << st.top() << std::endl;
        st.pop();
    }

    
    std::queue<std::string> q1;
    std::cout << "Q size " << q1.size() << std::endl;

    // priority queueu
    // heap, max heap by default
    //c: Underlying container. Uses vector as default.

    std::priority_queue<int , std::vector<int> , std::less<int> > pq;
    pq.push(10);
    pq.push(30);
    pq.push(2);
    pq.push(4);
    pq.push(430);
    pq.push(70);


    while(not pq.empty()){
      std::cout << "Priority Queue" << pq.top() << " " << std::endl;
      pq.pop();
    }

}

#include <algorithm>
struct Student{
    std::string name;
    int age;
    int roll;

    bool operator < (const Student &other) const {
        return (this->age < other.age);
    }

};
TEST(Algorithm, Basic) {
  std::vector<Student> vec = {
    {"Shyam Naarang", 39, 3},
    {"Ranjit Thakur", 44, 1},
    {"Aatma Prakash", 34, 2}
  };
  std::sort(vec.begin(), vec.end());
  for (auto k: vec)
    std::cout << "Vcetor aort" << k.name << std::endl;

  std::sort(vec.begin(), vec.end(),
    [] (auto x, auto y) {
      return (x.roll < y.roll) ;
    });
  for (auto k: vec)
    std::cout << "Lambda vector sort" << k.name << std::endl;


  if(std::binary_search(vec.begin(), vec.end(), Student{"Shyam Naarang", 39, 3}))
    std::cout << "It is present" << std::endl;
  else
    std::cout << "NOT present" << std::endl;

  std::list lk = {12,1,6,7727,8726,872,9982,7};
  //lk.reverse();
  auto itk  = lk.begin();
  std::advance(itk, 2);
  std::reverse(lk.begin(), itk);
  for (auto p:lk)
    std::cout << "List Lk : = " << p << std::endl;
  
}


TEST(ReverseList , Basic) {

  struct Node{
    int data;
    struct Node *next;

    Node(int _data): data(_data), next(nullptr){}
    
  };

  struct ListManager{
    Node *head = nullptr;
    Node *tail;
    

    ListManager(): head(nullptr), tail(nullptr){}
    void insert(int _data){
      auto temp = new Node(_data);
      temp->next = head;
      head = temp;
    }


    void display(){
      auto p = head;
      while(p != nullptr){
        std::cout << p->data << std::endl;
        p = p->next;
      }
    }


    void reverse(){
      Node *prev=nullptr, *curr=head, *next=curr;

      // a b c

      while(next != nullptr) {
        next = curr->next; // b next = c
        curr->next = prev; // a->next = null b->next = a
        prev = curr; // prev = a prev = b
        curr = next; // curr = b current = c
      }
      head = prev;
    }

  };
  ListManager l1;
  l1.insert(1);
  l1.insert(2);
  l1.insert(3);
  l1.insert(4);
  l1.insert(5);
  l1.insert(6);
  l1.insert(7);

  l1.display();
  l1.reverse();
  std::cout << "After reverse() " << std::endl;
  l1.display();



  // count of vector of strings , e.g One-3, Two-1 etc
  std::vector<std::string> vs = {
    "One",
    "Two",
    "One", 
    "One",
    "Two",
    "Three",
    "One",
    "Two",
    "One",
    "Three",
    "Two",
    "Four"
  };
  
  std::sort(vs.begin(), vs.end(), 
  [] (auto a, auto b){
    return (a < b);
  });

  std::string prev = vs[0];
  int count = 0;
  std::list< std::pair<std::string, int> > res;
  for (auto x:vs){
    // a, a,b,c, c
    if(prev == x)
    {
        count++;
        continue;
    }
    // b
    // push result for a
    // count for b = 1
    // update prev = b 
    res.push_back(std::make_pair<std::string &, int&>(prev, count) );
    count = 1;
    prev = x;
  }
  res.push_back(std::make_pair<std::string &, int&> (prev, count) );
  for (auto x: res)
    std::cout << x.first << " " << x.second << std::endl;

  // Alternate approach
  std::vector<std::string> vs2 = {
    "One",
    "Two",
    "One", 
    "One",
    "Two",
    "Three",
    "One",
    "Two",
    "One",
    "Three",
    "Two",
    "Four"
  };
  std::unordered_map<std::string, int> um2;
  for (auto x:vs2){
    um2[x]++;
  }

  std::cout << "Using unordered Map" << std::endl;
  for(auto x:um2)
    std::cout << x.first << " " << x.second << std::endl;


  // Count of different numbers
  std::vector v3 = {1,2,3,4,5,6,7,8,9,10,1,1,2,2,3,4,5,6,7,5,4,3,2,3,4,5};
  std::set<int> s3;
  
  for(auto x:v3)
    s3.insert(x);
  
  for(auto x:s3){
    auto count_element = std::count(v3.begin(), v3.end(), x);
    std::cout << "Element " << x << "Count :" << count_element << std::endl;
  } // Frequency table

  // Generate all permutations and store in a vector/other data structure
  std::list< std::vector<int> > lv; // list of vectors
  std::vector vn = {1,4,2,3,6,5};

  std::sort(vn.begin(), vn.end());
  int n = vn.size();
  int r=3;

  // select r elements from n elements
  // sort r elements
  // generate permutations of r elements, next_permutation()
  
  


}

std::recursive_mutex mtx;
std::mutex mtx2;
void recusrive(int depth){
  std::unique_lock<std::recursive_mutex> lock(mtx);
  if(depth <=0){
    return;
  }
  std::cout << "Depth " << depth << std::endl;
  recusrive(depth-1);
}


void recursive_non_reentrant(int depth){
  std::unique_lock<std::mutex> lock(mtx2);
  if(depth <=0){
    return;
  }
  std::cout << "Depth " << depth << std::endl;
  recursive_non_reentrant(depth-1);
}


void lock_deferred(){
  std::unique_lock<std::mutex> lock(mtx2, std::defer_lock);
  // do some work
  lock.lock();
  // do some work
  lock.unlock();
}

void transfer_lock(){
  std::unique_lock<std::mutex> lock1(mtx2);
  std::unique_lock<std::mutex> lock2(std::move(lock1));
  // lock1 is now empty, lock2 owns the mutex
  if(!lock1.owns_lock()){
    std::cout << "Lock1 does not own the mutex" << std::endl;
  }
  if(lock2.owns_lock()){
    std::cout << "Lock2 owns the mutex" << std::endl;
  }
}



void transfer_owner_ship(){
  std::mutex m1;
  m1.lock();
  std::unique_lock<std::mutex> lock(m1, std::adopt_lock);
  // lock now owns the mutex m1
  if(lock.owns_lock()){
    std::cout << "Lock owns the mutex m1" << std::endl;
  }

}
void try_lock_for_example(){
  std::timed_mutex mtx3;
  std::unique_lock<std::timed_mutex> lock(mtx3);
  if(lock.try_lock_for(std::chrono::seconds(2))){
    std::cout << "Lock acquired" << std::endl;
    mtx3.unlock();
  } else {
    std::cout << "Failed to acquire lock after 1 second" << std::endl;
  }
}
TEST(Recursive_Unique, Basic) {
  //recusrive(5);
  //recursive_non_reentrant(5); // will cause deadlock as the same thread tries to acquire the lock it already holds
  // lock_deferred();
  // transfer_lock();
  // transfer_owner_ship();

  // std::thread t1(try_lock_for_example);
  // std::thread t2(try_lock_for_example);
  // t1.join();
  // t2.join();
  
}



std::mutex q_mtx;
std::queue<int> q;
std::condition_variable q_cv;
#define MAX_SIZE_Q 57


void producer(int val) {
  while(val){
    std::unique_lock<std::mutex> lock(q_mtx);
    q_cv.wait(lock, []() { return q.size() < MAX_SIZE_Q; }); // wait until queue has space
    q.push(val);
    std::cout << "Produced: " << val << std::endl;
    val--;
    lock.unlock();
    q_cv.notify_one(); // notify consumer
  }
}


void consumer() {
  while(true){
    std::unique_lock<std::mutex> lock(q_mtx);
    q_cv.wait(lock, []() { return !q.empty(); }); // wait until queue has items
    int val = q.front();
    q.pop();
    std::cout << "Consumed: " << val << std::endl;
    lock.unlock();
    q_cv.notify_one(); // notify producer
  }
}

TEST(Recursive_Unique, Prod_Cons) {

  std::thread prod(producer, 100);
  std::thread cons(consumer);

  prod.join();
  cons.detach(); // consumer runs indefinitely, so we detach it

}

TEST(basic, Basic) {
  std::cout << "Hello World test" << std::endl;

}
#include <future>
#include <chrono>


TEST(Async, basic) {
  std::future<uint64_t> fut = std::async(std::launch::async, [](int n) {
    uint64_t sum = 0;
    for (uint64_t i = 0; i < n; ++i) {
      sum += i;
    }
    return sum;
  }, 100);

  // Do some other work while the async task is running
  std::cout << "Doing other work in main thread..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  uint64_t result = fut.get();
  std::cout << "Result of async computation: " << result << std::endl;

}

TEST(move_semantics, basic) {
  std::vector<int> v1 = {1,2,3,4,5};

  std::vector<int> v2 = std::move(v1);
  std::cout << "v2 size: " << v2.size() << std::endl; // v2 has the elements
  std::cout << "v1 size: " << v1.size() << std::endl; // v1 is empty or in a valid but unspecified state
}


TEST(variadic_templates, basic) {
  
  auto product = [](auto... args) {
    return (args * ...); // fold expression to multiply all arguments
  };


  std::cout << "Product of 1,2,3 is: " << product(1,2,3) << std::endl;
  std::cout << "Product of 10,20 is: " << product(10,20) << std::endl;
  std::cout << "Product of 5,10,15,20 is: " << product(5,10,15,20) << std::endl;


  auto sum = [] (auto... args) {
    return (args + ...); // fold expression to sum all arguments
  };

  std::cout << "Sum of 1,2,3 is: " << sum(1,2,3) << std::endl;
  std::cout << "Sum of 10,20 is: " << sum(10,20) << std::endl;
  std::cout << "Sum of 5,10,15,20 is: " << sum(5,10,15,20) << std::endl;
}

TEST(variadic_templates, recursive) {
  // Recursive variadic template to calculate factorial
  std::function<int(int)> factorial = [&factorial](int n) -> int {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
  };

  std::cout << "Factorial of 5 is: " << factorial(5) << std::endl;
  std::cout << "Factorial of 10 is: " << factorial(10) << std::endl;
}

TEST(stdref, basic) {
  std::vector<int> v = {1,2,3,4,5};
  std::vector<std::reference_wrapper<int>> ref_vec;
  for (auto &x:v)
    ref_vec.push_back(std::ref(x)); // store reference to x in ref_vec

  for (auto &ref:ref_vec)
    std::cout << "Reference wrapper value: " << ref.get() << std::endl; // access the value through get()
}
#include <string_view>
TEST(stringview, basic) {
  std::string str = "Hello, World!";
  std::string_view sv(str); // create a string_view that references str

  std::cout << "String view: " << sv << std::endl; // prints "Hello, World!"
  std::cout << "Size of string view: " << sv.size() << std::endl; // prints the size of the string view

  // Modifying the original string will affect the string_view
  // why string view? 
  // because it is a non-owning reference to a string, 
  // it does not manage the memory of the string, 
  // it is lightweight and efficient for read-only access to strings, 
  // it can be used to avoid unnecessary copying of strings, and it can be used to create substrings without copying the original string.

  str[7] = 'C';
  std::cout << "Modified string view: " << sv << std::endl; // prints "Hello, Corld!"
}


 std::vector<int> createVector() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    return vec; // Copy elision may occur here
  }

TEST(democopyelision, moderate){
  // Copy elision is an optimization technique where the compiler can eliminate unnecessary copy and move operations, 
  // even if the copy/move constructor has side effects. 
  // This can lead to improved performance by avoiding the overhead of copying or moving objects.

  // Example of copy elision:
 
  // what happened above is that the compiler can optimize the return statement to construct the vector directly in the memory location where it will be used,
  // meaning of eiision is that the compiler can eliminate the temporary object that would normally be created when returning a value from a function, 
  // and instead construct the object directly in the memory location where it will be used.

  std::vector<int> myVec = createVector(); // No copy or move constructor is called here due to copy elision
  for (int num : myVec) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
}


TEST(demouniquelock,defer_lock){
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx, std::defer_lock); // Create a unique_lock that does not acquire the mutex immediately

  // Do some work here without holding the lock

  lock.lock(); // Now acquire the lock when needed
  // Critical section protected by the mutex
  std::cout << "Lock acquired, doing critical work..." << std::endl;

  lock.unlock(); // Release the lock when done
}

TEST(demouniquelock, transfer_ownership){
  std::mutex mtx;
  std::unique_lock<std::mutex> lock1(mtx); // lock1 acquires the mutex
  std::unique_lock<std::mutex> lock2(std::move(lock1)); // Transfer ownership from lock1 to lock2

  if (!lock1.owns_lock()) {
    std::cout << "lock1 does not own the mutex" << std::endl;
  }
  if (lock2.owns_lock()) {
    std::cout << "lock2 owns the mutex" << std::endl;
  }

  // lock2 will automatically release the mutex when it goes out of scope
}


TEST(deomstdlock, stdlock){
  std::mutex mtx1, mtx2;
  std::thread t1([&]() {
    std::lock(mtx1, mtx2); // Lock both mutexes without deadlock
    std::cout << "Thread 1 has locked both mutexes" << std::endl;
    mtx1.unlock();
    mtx2.unlock();
  });

  std::thread t2([&]() {
    std::lock(mtx1, mtx2); // Lock both mutexes without deadlock
    std::cout << "Thread 2 has locked both mutexes" << std::endl;
    mtx1.unlock();
    mtx2.unlock();
  });

  t1.join();
  t2.join();
}


TEST(demofuture, basic){
  std::promise<uint64_t> prom_to_calculate_sum;
  auto future_sum = prom_to_calculate_sum.get_future();
  // Now calculate the sum
  std::thread t1([](std::promise<uint64_t> &&promise, uint64_t start, uint64_t end) 
  {
    uint64_t sum=0;
    for(uint64_t i=start; i< end;i++){
      sum += i;
    }
    promise.set_value(sum); // Set the value of the promise

  }, std::move(prom_to_calculate_sum), 0, 100);

  std::cout << "Demo future - promise Sum = " << future_sum.get() << std::endl;
  t1.join();

}


  class Rectangle {
  protected:
    int width;
    int height;
  public:
    Rectangle(int w, int h) : width(w), height(h) {}
    virtual int area() const { return width * height; }
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
  };

  class Square : public Rectangle {
  public:
    Square(int side) : Rectangle(side, side) {}
    void setWidth(int w) override {
      width = w;
      height = w; // Ensure height is always equal to width
    }
    void setHeight(int h) override {
      height = h;
      width = h; // Ensure width is always equal to height
    }
  };


TEST(demoliskovssubstitution, violatesliskovsubstitution){ 
  // Liskov Substitution Principle states that objects of a superclass should be replaceable with objects of a subclass without affecting the correctness of the program. 
  // In other words, if S is a subtype of T, then objects of type T may be replaced with objects of type S without altering any of the desirable properties of the program (correctness, task performed, etc.).
  // define a function to justify why square should not inherit rectangle
    auto calculateArea = [](Rectangle &rect) {
      rect.setWidth(10);
      rect.setHeight(5);
      std::cout << "Area: " << rect.area() << std::endl; // Expected area is 50 for a rectangle
    };


  Rectangle rect(0,0);
  calculateArea(rect); // This will work fine and print Area: 50

  Square sq(0);
  calculateArea(sq); // This will violate Liskov Substitution Principle and print Area
  // because when we set width to 10 and height to 5 for the square, it will actually set both width and height to 10 (or both to 5),
  // which will lead to an incorrect area calculation (100 or 25 instead of 50), thus violating the principle that a Square should be substitutable for a Rectangle without affecting the correctness of the program.

}


class GeometricFigure {
public:
    virtual int area() const = 0;
    virtual ~GeometricFigure() = default;
};


class Rectangle_N : public GeometricFigure {
private:
    int width;
    int height;

public:
    Rectangle_N(int w, int h) : width(w), height(h) {}

    int area() const override {
        return width * height;
    }
};
class Square_N : public GeometricFigure {
private:
    int side;

public:
    explicit Square_N(int s) : side(s) {}

    int area() const override {
        return side * side;
    }
};



TEST(demosolvelsp, solveaboveusingGeometricFigureInterface){

  auto printArea = [](const GeometricFigure &shape){
    std::cout << "Area: " << shape.area() << std::endl;
  };

  Rectangle_N r(5, 10);
  Square_N s(5);

  printArea(r);  // 50
  printArea(s);  // 25

}

TEST(stdvariantdemo, basic){
  std::variant<int, std::string> var;// var can hold either an int or a string
  var = 42; // var holds an int
  std::cout << "Variant holds int: " << std::get<int>(var) << std::endl;

  var = "Hello, Variant!"; // var now holds a string
  std::cout << "Variant holds string: " << std::get<std::string>(var) << std::endl;

  // Using std::visit to handle the variant
  std::visit([](auto&& arg){
    using T = std::decay_t<decltype(arg)>;// deduce the type of arg, decay_t removes references and cv-qualifiers
    if constexpr (std::is_same_v<T, int>) {
      std::cout << "Visited int: " << arg << std::endl;
    } else if constexpr (std::is_same_v<T, std::string>) {
      std::cout << "Visited string: " << arg << std::endl;
    }
  }, var);
}

TEST(simpleexamplestdvariant, basic){
  std::variant<int, double, std::string> var;
  var = 10; // holds an int
  std::cout << "Variant holds int: " << std::get<int>(var) << std::endl;

  var = 3.14; // holds a double
  std::cout << "Variant holds double: " << std::get<double>(var) << std::endl;

  var = "Hello, World!"; // holds a string
  std::cout << "Variant holds string: " << std::get<std::string>(var) << std::endl;
}

  template<typename... Types>
  using MyVariant = std::variant<Types...>;

TEST(variantwithvariadictemplate, basic) {
  // std::variant can also be used with variadic templates to create a variant that can hold any type from a list of types. 
  // This allows for more flexibility and can be useful in situations where you want to store different types of data in a single variable.



  MyVariant<int, double, std::string> var;
  var = 42; // holds an int
  std::cout << "MyVariant holds int: " << std::get<int>(var) << std::endl;

  var = 3.14; // holds a double
  std::cout << "MyVariant holds double: " << std::get<double>(var) << std::endl;

  var = "Hello, Variadic Variant!"; // holds a string
  std::cout << "MyVariant holds string: " << std::get<std::string>(var) << std::endl;
}
  inline constexpr int MAX_SIZE = 100; // Define an inline constexpr variable


TEST(democonstexprwithinline, basic) {
  // constexpr with inline variables allows you to define constants that can be evaluated at compile time 
  // and are also inline, meaning they can be defined in header files without violating the 
  // One Definition Rule (ODR). 
  // This is particularly useful for defining constants that need to be shared across multiple translation units.


  std::cout << "Max size: " << MAX_SIZE << std::endl; // Use the constant
}