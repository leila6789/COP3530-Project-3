#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>


using namespace std;

//Initializing variables from the data file
struct TableRecord {
  int year;
  string gender;
  string intent;
  string drug_type;
  int age0;
  int age1;
  int age5;
  int age15;
  int age25;
  int age35;
  int age45;
  int age55;
  int age65;
  int age75;
  int age85;
  int age_unknown;
  int age_all;
};



int to_int(const string &s) {
  int i = 0;
  try {
    i = stoi(s);
  }
  //catches if there is no data in table
  //nothing will happen
  catch (exception &e) {
    //cout << "invalid number ";
  }
  return i;
}
//Gathering the ascending order of the categories
bool compByAgeAll(const TableRecord &a, const TableRecord &b) {
  return a.age_all < b.age_all;
}

bool compByDrugType(const TableRecord &a, const TableRecord &b) {
  return a.drug_type < b.drug_type;
}

bool compByGender(const TableRecord &a, const TableRecord &b) {
  return a.gender < b.gender;
}

bool compByYearAsc(const TableRecord &a, const TableRecord &b) {
  return a.year < b.year;
}

bool compByYearDesc(const TableRecord &a, const TableRecord &b) {
  return a.year > b.year;
}
//Getting the data from the file
//Some require conversions i.e. the ages
istream &operator>>(istream &in, TableRecord &rec) {
  string temp;
  getline(in, temp, '\t');
  rec.year = to_int(temp);
  getline(in, rec.gender, '\t');
  getline(in, rec.intent, '\t');
  getline(in, rec.drug_type, '\t');
  getline(in, temp, '\t');
  rec.age0 = to_int(temp);
  getline(in, temp, '\t');
  rec.age1 = to_int(temp);
  getline(in, temp, '\t');
  rec.age5 = to_int(temp);
  getline(in, temp, '\t');
  rec.age15 = to_int(temp);
  getline(in, temp, '\t');
  rec.age25 = to_int(temp);
  getline(in, temp, '\t');
  rec.age35 = to_int(temp);
  getline(in, temp, '\t');
  rec.age45 = to_int(temp);
  getline(in, temp, '\t');
  rec.age55 = to_int(temp);
  getline(in, temp, '\t');
  rec.age65 = to_int(temp);
  getline(in, temp, '\t');
  rec.age75 = to_int(temp);
  getline(in, temp, '\t');
  rec.age85 = to_int(temp);
  getline(in, temp, '\t');
  rec.age_unknown = to_int(temp);
  getline(in, temp);
  rec.age_all = to_int(temp);
  return in;
}
//Writing the data variables
ostream &operator<<(ostream &out, const TableRecord &rec) {
  out << rec.year << '\t' << rec.gender << '\t' << rec.intent << '\t'
      << rec.drug_type << '\t' << rec.age0 << '\t' << rec.age1 << '\t'
      << rec.age5 << '\t' << rec.age15 << '\t' << rec.age25 << '\t' << rec.age35
      << '\t' << rec.age45 << '\t' << rec.age55 << '\t' << rec.age65 << '\t'
      << rec.age75 << '\t' << rec.age85 << '\t' << rec.age_unknown << '\t'
      << rec.age_all;
  return out;
}

//Adopted from https://www.geeksforgeeks.org/quick-sort/ with modifications
//Start quicksort
template <typename T, typename Pred>
int partition(vector<T> &v, int low, int high, Pred comp) {
  int mid = (low + high) / 2;
  T pivot = v[mid];
  swap(v[mid], v[high]);

  int i = (low - 1);
  for (int j = low; j <= high - 1; j++) {
    if (comp(v[j], pivot)) {
      i++;
      swap(v[i], v[j]);
    }
  }
  swap(v[i + 1], v[high]);
  return (i + 1);
}

//Adopted from https://www.geeksforgeeks.org/quick-sort/ with modifications
template <typename T, typename Pred>
void quick_sort(vector<T> &v, int low, int high, Pred comp) {
  if (low < high) {
    int pivot_index = partition(v, low, high, comp);
    quick_sort(v, low, pivot_index - 1, comp);
    quick_sort(v, pivot_index + 1, high, comp);
  }
}
//End quicksort



//resource used to learn about radix sort: 
//https://www.simplilearn.com/tutorials/data-structure-tutorial/radix-sort#:~:text=Radix%20Sort%20algorithm%20is%20a%20stable%20sorting%20subroutine%2Dbased%20integer,same%20significant%20position%20and%20value.

void radix_sort(vector<TableRecord>& arr, string category) {
    //find max value in data set for each cateory
    int max_val = 0;
    if (category == "year") {
        for (auto& rec : arr) {
            max_val = max(max_val, rec.year);
        }
    } else if (category == "gender") {
        for (auto& rec : arr) {
            max_val = max(max_val, static_cast<int>(rec.gender[0]));
        }
    } else if (category == "drug_type") {
        for (auto& rec : arr) {
            max_val = max(max_val, static_cast<int>(rec.drug_type[0]));
        }
    }
  else if (category=="age"){
     for (auto& rec : arr) {
            max_val = max(max_val, rec.age_all);
        }
  }
     else {
        throw invalid_argument("Invalid category number for radix sort");
    }

    //starting from the least significant digit
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        vector<TableRecord> output(arr.size());
        vector<int> count(256, 0);

        //Counting frequency of each digit
        for (auto& rec : arr) {
            int index = 0;
            if (category == "year") {
                index = (rec.year / exp) % 10;
            } else if (category == "gender") {
                index = static_cast<int>(rec.gender[0]) / exp % 10;
            } else if (category == "drug_type") {
                index = static_cast<int>(rec.drug_type[0]) / exp % 10;
            }
            else if (category=="age"){
              index=(rec.age_all/exp) %10;

            }
            count[index]++;
        }

        //convert to position
        for (int i = 1; i < count.size(); i++) {
            count[i] += count[i - 1];
        }

        //make output array
        for (int i = arr.size() - 1; i >= 0; i--) {
            int index = 0;
            if (category == "year") {
                index = (arr[i].year / exp) % 10;
            } else if (category == "gender") {
                index = static_cast<int>(arr[i].gender[0]) / exp % 10;
            } else if (category == "drug_type") {
                index = static_cast<int>(arr[i].drug_type[0]) / exp % 10;
            }
            else if (category=="age"){
              index=(arr[i].age_all/exp) % 10;
            }
            output[count[index] - 1] = arr[i];
            count[index]--;
        }

        //copying the output array to the original
        for (int i = 0; i < arr.size(); i++) {
            arr[i] = output[i];
        }
    }
}


int main() {
  ifstream inputFile("more_drug_rates_excel.txt");
  vector<TableRecord> table;
  TableRecord rec;
  int category;

  while (inputFile >> rec) {
    table.push_back(rec);
  }
  inputFile.close();
while(true){
   //ask user which sort they want done
  std::cout << "Choose a sorting algorithm:" << endl;
  std::cout << "1. Quick sort" << endl;
  std::cout << "2. Radix sort" << endl;
  int sortChoice;
 std:: cin >> sortChoice;
//picking category to sort with
 std:: cout << "Choose a category to sort by:" << endl;
  std::cout << "1. Age" << endl;
  std::cout << "2. Drug type" << endl;
  std::cout << "3. Gender" << endl;
  std::cout << "4. Year" << endl;
  int categoryChoice;
 std:: cin >> categoryChoice;
//Sort data from choices
  switch (sortChoice) {
    case 1:
      switch (categoryChoice) {
        case 1: {
          auto start_quick = chrono::steady_clock::now();
          quick_sort(table, 0, table.size() - 1, compByAgeAll);
          auto end_quick = chrono::steady_clock::now();
          std::cout << "Time taken by Quick Sort: " << chrono::duration_cast<chrono::microseconds>(end_quick - start_quick).count() << "Ms" << endl;
          break;
        }
        case 2: {
          auto start_quick = chrono::steady_clock::now();
          quick_sort(table, 0, table.size() - 1, compByDrugType);
          auto end_quick = chrono::steady_clock::now();
          std::cout << "Time taken by Quick Sort: " << chrono::duration_cast<chrono::microseconds>(end_quick - start_quick).count() << "Ms" << endl;
          break;
        }
        case 3: {
          auto start_quick = chrono::steady_clock::now();
          quick_sort(table, 0, table.size() - 1, compByGender);
          auto end_quick = chrono::steady_clock::now();
          std::cout << "Time taken by Quick Sort: " << chrono::duration_cast<chrono::microseconds>(end_quick - start_quick).count() << "Ms" << endl;
          break;
        }
        case 4: {
          auto start_quick = chrono::steady_clock::now();
          quick_sort(table, 0, table.size() - 1, compByYearAsc);
          auto end_quick = chrono::steady_clock::now();
          std::cout << "Time taken by Quick Sort: " << chrono::duration_cast<chrono::microseconds>(end_quick - start_quick).count() << "Ms" << endl;
          break;
        }
        default: {
         std:: cout << "Invalid category choice";
        }
      }
      for (auto &rec : table) {
    std::cout << rec << endl;
  }
      break;
   case 2:
      switch (categoryChoice) {
        case 1: {
          auto start_radix = chrono::steady_clock::now();
          radix_sort(table, "age");
          auto end_radix = chrono::steady_clock::now();
          std::cout << "Time taken by Radix Sort: " << chrono::duration_cast<chrono::microseconds>(end_radix - start_radix).count() << "Ms" << endl;
          break;
        }
        case 2: {
          auto start_radix = chrono::steady_clock::now();
          radix_sort(table, "drug_type");
          auto end_radix = chrono::steady_clock::now();
          std::cout << "Time taken by Radix Sort: " << chrono::duration_cast<chrono::microseconds>(end_radix - start_radix).count() << "Ms" << endl;
          break;
        }
        case 3: {
          auto start_radix = chrono::steady_clock::now();
          radix_sort(table, "gender");
          auto end_radix = chrono::steady_clock::now();
          std::cout << "Time taken by Radix Sort: " << chrono::duration_cast<chrono::microseconds>(end_radix - start_radix).count() << "Ms" << endl;
          break;
        }
        case 4: {
          auto start_radix = chrono::steady_clock::now();
          radix_sort(table, "year");
          auto end_radix = chrono::steady_clock::now();
          std::cout << "Time taken by Radix Sort: " << chrono::duration_cast<chrono::microseconds>(end_radix - start_radix).count() << "Ms" << endl;
          break;
        }
        default: {
          std::  cout << "Invalid category choice";
        }
      }
  for (auto &rec : table) {
    std::cout << rec << endl;
  }
  break;
}
}
}