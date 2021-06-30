#include "generator_simple.h"
#include <cstdio> 
#include <functional> 
#include <string>           
#include <vector>   
   
using namespace std; // NOLINT(google-build-using-namespace)
 
class TPrintSE { // NOLINT(cppcoreguidelines-special-member-functions,hicpp-special-member-functions)
  string strEnd;
public: 
  TPrintSE(const string &Start_, string End_): strEnd(move(End_)) { 
    printf("%s", Start_.c_str());
  }
  ~TPrintSE() {
    printf("%s", strEnd.c_str());
  } 
};

vector<coroexp::generator<int>> TestGens; // NOLINT(fuchsia-statically-constructed-objects, cppcoreguidelines-avoid-non-const-global-variables)
vector<coroexp::generator<int>> NewGens;  // NOLINT(fuchsia-statically-constructed-objects, cppcoreguidelines-avoid-non-const-global-variables)

void AddGenAuto(auto NewTest) {
  NewGens.push_back(move(NewTest()));
}  

void AddGenFunc(function<coroexp::generator<int>()> NewTest) { // NOLINT (performance-unnecessary-value-param)
  NewGens.push_back(move(NewTest()));
}   
 
void AddGenRef(auto &NewTest) { 
  NewGens.push_back(NewTest());
}
  
template<class T>
T &&AddGenAutoPass(T &&NewTest) {
  NewGens.push_back(NewTest());
  return forward<T>(NewTest);
}

void UpdateGens() {
  for (auto &x:NewGens) {
    TestGens.push_back(move(x));
  }
  NewGens.clear();
}

coroexp::generator<int> TestGen() {
  auto SE2 = TPrintSE("Outer Gen - Start\n", "Outer Gen - Destroy\n");
  printf("Outer Gen - Start, AddGen\n");
  int val = 12; // NOLINT (cppcoreguidelines-avoid-magic-numbers)

  // Fails: The Lambda is stored in a temporary, and therefore goes out of scope before the coroutine itself starts
  AddGenAuto([val, SE = TPrintSE("Inner Gen Auto - Capture Init\n", "Inner Gen Auto - Capture Destroy\n")]()->coroexp::generator<int> {
    auto SE2a = TPrintSE("Inner Gen Auto - Start\n", "Inner Gen Auto - Destroy\n");
    printf("Inner Gen Auto - Start, Yield, Captured value (should be 12): %i\n", val);
    co_yield 2;
    printf("Inner Gen Auto - End\n"); 
  });

  // Fails: The Lambda is stored in a temporary, and therefore goes out of scope before the coroutine itself starts. However, due to the way std::function stores captures, the memory might not be overwritten yet while running the coroutine.
  AddGenFunc([val, SE = TPrintSE("Inner Gen Func - Capture Init\n", "Inner Gen Func - Capture Destroy\n")]()->coroexp::generator<int> {
    auto SE2b = TPrintSE("Inner Gen Func - Start\n", "Inner Gen Func - Destroy\n");
    printf("Inner Gen Func - Start, Yield, Captured value (should be 12): %i\n", val);
    co_yield 3;
    printf("Inner Gen Func - End\n");
  });

  // Succeeds: The Lambda remains in scope, since it is stored in 'L' rather than any temporary
  auto L = [val, SE = TPrintSE("Inner Gen Ref - Capture Init\n", "Inner Gen Ref - Capture Destroy\n")]()->coroexp::generator<int> {
    auto SE2c = TPrintSE("Inner Gen Ref - Start\n", "Inner Gen Ref - Destroy\n");
    printf("Inner Gen Ref - Start, Yield, Captured value (should be 12): %i\n", val);
    co_yield 4;
    printf("Inner Gen Ref - End\n");
  };
  AddGenRef(L);

  // Fails: Probably because despite forwarding, the original Lambda is still lost, and instead a new copy is created
  auto L2 = AddGenAutoPass([val, SE = TPrintSE("Inner Gen AutoPass - Capture Init\n", "Inner Gen AutoPass - Capture Destroy\n")]()->coroexp::generator<int> {
    auto SE2d = TPrintSE("Inner Gen AutoPass - Start\n", "Inner Gen AutoPass - Destroy\n");
    printf("Inner Gen AutoPass - Start, Yield, Captured value (should be 12): %i\n", val);
    co_yield 5; // NOLINT (cppcoreguidelines-avoid-magic-numbers)
    printf("Inner Gen AutoPass - End\n");
  });

  printf("Outer Gen - Yield\n");
  co_yield 1;
  printf("Outer Gen - End\n");
}

void CaptureTest() {
  AddGenAuto(TestGen);
  UpdateGens();
  TestGens[0].next();
  UpdateGens();
  TestGens[1].next();
  TestGens[2].next();
  TestGens[3].next();
  TestGens[4].next();
  TestGens.clear();
}

int main() {
  CaptureTest();
  printf("## Finished\n");
  return 0;
}