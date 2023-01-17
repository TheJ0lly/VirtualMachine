#include "../include/Stack/Stack.hpp"
#include "../include/VM/VirtualMachine.hpp"

int main()
{
    VM vm;

    //We allocate the memory for the string "TestTest", which will be from 0 - 8, including terminator.
    Var a = vm.allocateMemory("TestTest", 's');

    vm.print(a);
    std::cout << a.address << " - " << a.type << "\n" << std::endl;

    //We allocate the memory for the string "TestTest2", which will be from 9 - 18, including terminator.
    Var b = vm.allocateMemory("TestTest2", 's');
    vm.print(b);
    std::cout << b.address << " - " << b.type << "\n" << std::endl;
    
    //Here we deallocate the memory of variable 'a', which is from 0 - 8, including terminator.
    vm.deallocateMemory(a);

    //Here we allocate the memory for the string "6969", which will be from 0 - 4, including terminator.
    Var k = vm.allocateMemory("6969", 's');
    vm.print(k);
    std::cout << k.address << " - " << k.type << "\n" << std::endl;

    //Here we allocate the memory for the string "KA", which will be from 5 - 8, including terminator.
    Var p = vm.allocateMemory("KA", 's');
    vm.print(p);
    std::cout << p.address << " - " << p.type << "\n" << std::endl;

    //Here we deallocate k and p to make way for the new array
    vm.deallocateMemory(k);
    vm.deallocateMemory(p);


    //Here we allocate the memory for the array of strings, which will contain 
    Array arr = vm.allocateMemoryArray({"Test1", "Test2"}, 's');
    //Here we can see that the addresses are in different locations in memory, but stored next to one another inside of the array.
    std::cout << "arr addresses: " << arr.addresses[0] << " - " << arr.addresses[1] << "\n" << std::endl;

    for (int i = 0; i < arr.addresses.size(); i++)
    {
        vm.print(arr.at(i));
        std::cout << arr.at(i).address << " - " << arr.at(i).type << "\n" << std::endl;
    }

    //Here we deallocate all the memory used by the array
    vm.deallocateMemoryArray(arr);

    //Here we will get 2 errors: TRYING TO ACCESS UNALLOCATED MEMORY and TRIED PRINTING UNUSED MEMORY, because we deallocated the memory of 'arr' and because we try to print something from a not-used block.
    vm.print(arr.at(1));
}