#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <memory>
using namespace std;

class myclass {
    public:
        myclass() {
            cout << "default constructor";
        }
}

int main() {
    // int arr[3];
    // arr[0] = 1;
    // arr[1] = 2;
    vector<int> arr(3);
    arr[3] = 4 //this will throw an out of bounds error instead of a segfault or no ... who fuckin knows
    arr.push_back(1); //adds to back

    // for(size_t i = 0; i < count; i++)
    // {
    //     /* code */
    // }

    // can also use enchanced for loops

    // for(auto&& i : v)
    // {
        
    // }
    //SETS
    // if you make it a set then other stuff happens, you dont have push back... duh. But you can insert
    //preserves insert order


    // maps are basically hashmaps wow cool wow... key value pairs
    map<string, string> my_map;
    my_map["hello"] = "world";
    my_map["files"] = "stream";

    for(auto [key,value] : my_map)
    {
        cout << key << " " << value;
    }
    
    //basically python lmfao
    auto [firstre, secondre] = foo2();



    //smart pointers lmaoooo
    myclass* blah = new myclass();

    return 0;
}
//this changes it because you're passing by reference. 
string foo(stirng &alsooutput) {
    alsooutput = "second value";
    return "one value";
}



pair<string, string> foo2() {
    return make_pair("one value", "second value");
}