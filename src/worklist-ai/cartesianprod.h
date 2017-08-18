// Use of the CartesianProduct class is as follows. Give it the number
// of rows and the sizes of each of the rows. It will output all of the 
// permutations of these numbers in their respective rows.
// 1. call cp.permutation() // need to check all 0s.
// 2. while cp.HasNext() // it knows the exit condition form its inputs.
// 3.   cp.Increment() // Make the next permutation
// 4.   cp.permutation() // get the next permutation
#include <vector>
#include <iostream>

class CartesianProduct{
  public:
  CartesianProduct(std::vector<int> sizes_of_rows){
    //permutation_ = new int[num_rows];
    num_rows_ = sizes_of_rows.size();
    ZeroOutPermutation();
    sizes_of_rows_ = sizes_of_rows;
    num_max_permutations_ = 1;
    for (int i = 0; i < num_rows_; ++i){
      assert(sizes_of_rows_[i] != 0 && "size zero row");
      num_max_permutations_ *= sizes_of_rows_[i]; 
    }
  }

  ~CartesianProduct(){
    //delete permutation_;
  }

  bool HasNext(){
    if(num_permutations_processed_ != num_max_permutations_) {
      return true;
    } else {
      return false;
    }
  }

  bool AtEnd() {
    if (num_permutations_processed_ > num_max_permutations_) {
      return true;
    }
    else {
      return false;
    }
  }

 void Increment(){
    int row_to_increment = 0;
    ++num_permutations_processed_;
    if (!AtEnd()) {
      IncrementAndTest(row_to_increment);
    }
  }

 std::vector<int> permutation(){
    return permutation_;
  }

  int num_permutations_processed(){
    return num_permutations_processed_;
  }
  void PrintPermutation(){
    std::cout << "( ";
    for (int i = 0; i < num_rows_; ++i){
      std::cout << permutation_[i] << ", ";
    }
    std::cout << " )" << std::endl;
  }

private:
  int num_permutations_processed_;
  std::vector<int> permutation_;
  int num_rows_;
  int num_max_permutations_;
  std::vector<int> sizes_of_rows_;

  // Because CartesianProduct is called first initially with it's values
  // of 0 and because those values are valid and important output
  // of the CartesianProduct we increment the number of permutations
  // processed here when  we populate the permutation_ array with 0s.
  void ZeroOutPermutation(){
    assert(num_rows_ != 0);
    for (int i = 0; i < num_rows_; ++i){
      //permutation_[i] = 0;
      permutation_.push_back(0);
    }
    num_permutations_processed_ = 1;
  }

  void IncrementAndTest(int row_to_increment){
    permutation_[row_to_increment] += 1;
    int max_index_of_row = sizes_of_rows_[row_to_increment] - 1;
    if (permutation_[row_to_increment] > max_index_of_row){
      permutation_[row_to_increment] = 0;
      IncrementAndTest(row_to_increment + 1);
    }
  }
};
