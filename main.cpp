#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>


using namespace std;

const int user_array_size = 35190;
const int item_array_size = 7335;
const int train_line_count = 1123345;






/*
To see the total memory usage of your C++ code, you can use the std::malloc_stats() function.
This function will print out statistics about the current state of the memory allocator,
including the total amount of memory that has been allocated and the number of blocks of memory that are currently in use.
Keep in mind that this function is not part of the standard C++ library, so you will need to include the <malloc.h> header file in order to use it.



Once you have deleted an object in C++, the memory that it was using will be freed up and can be used by other parts of your program.
However, if your program no longer has any use for that memory, it will not be automatically returned to the operating system.
In order to return the memory to the operating system, you can either delete the entire array or use a function like std::vector::shrink_to_fit() to release any unused memory.
Keep in mind that returning memory to the operating system is not always necessary and can actually reduce the performance of your program if it needs to frequently allocate and deallocate memory.

 */

/*
 *
 *
 *
 * karşılaştırma, tahmin aşaması
 *
 *
 *
 *
 * movie_rating_counts arrayi heapify edilecek ve top 10 bulunmuş olacak
 *
 * userların arrayi heapify edilecek ve top 10 bulunmuş olacak
 *
 *
 */












//////////////////////////////////////////////////////////////////////////////////////////////////

//top 10 için obje
class tempobject {
public:
    int id;
    int count;

    //arraye yerleşmiş olacak olan objeleri bu fonksiyon ile atayacağız
    void set(int Id, int Count)
    {
        id = Id;
        count = Count;
    }

    tempobject() {
        id = 2147483647;
        count = 0;
    };

};

//////////////////////////////////////////////////////////////////////////////////////////////////


//userları ve userların içinde ratingleri depolayacağımız data structureımız

template <typename T>
class storage {
public:
    T* array;
    int last_object_index;

    //tek değişkenli constructorlarda bug olmaması için explicit keywordü kullanılıyormuş
    explicit storage(int size)
    {
    array = new T[size];
    last_object_index = 0;
    }


    //user ve rating objeleri için next_object_idleri halleder:
    //userların tutulduğu arrayde, userların içindeki next_object_index değişkenini hallediyor.
    //ratinglerin tutulduğu arrayde, rating_classlarının içindeki next_object_index değişkenini hallediyor.
    void handle_next_object_index(int id) {
        if (last_object_index != id) {
            if (last_object_index != 0)
            {
                array[last_object_index].next_object_index = id;
            }
        }
        last_object_index = id;
    }


    //userlar classının içinde, rating_arraye ekleme yapmak istendiğinde çağırılacak fonksiyon
    void add_to_rating_list(int itemid, float rating)
    {
        handle_next_object_index(itemid);
        array[itemid].rate = rating;
    }

    // userların tutulduğu arrayde userın rating ekleme fonksiyonunu çağırırız
    void add_to_user(int userid, int itemid, float rating)
    {
        handle_next_object_index(userid);
        array[userid].add_rating(itemid, rating);
    }



    //new keyword ü ile oluşturulan listeyi bu şekilde silmek gerekiyor
    ~storage()
    {
        delete[] array;
    }
};


//////////////////////////////////////////////////////////////////////////////////////////////////


//Userın içindeki storage objesinde ratingleri bunun içinde tutacağız
//içinde bulunduğu arrayde boşlukları atlayabilmek için nex_object_index ve ratingi tutuyor

class rating_class {
public:
    float rate;
    int next_object_index;

    //tek değişkenli constructorlarda bug olmaması için explicit keywordü kullanılıyormuş
    explicit rating_class(float rating = -1.0) {
        rate = rating;
        next_object_index = 0;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////

//user listin içinde bulunacak userların objesi
//içinde movielerin ratinglerini tutan bir obje
//ve kullanıcının kaç rating yaptığını gösteren değişken var
//rating eklerken rating alan filmin de movie_rating_counts arrayinde sayısını arttırıyor

class user {
public:

    int next_object_index;
    storage<rating_class> rating_array_class = storage<rating_class>(item_array_size);

    user()
    {
        next_object_index = 0;
    }
    void add_rating(int itemid, float rating) {
        rating_array_class.add_to_rating_list(itemid, rating);
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////

void userHeapify(tempobject arr[], int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l].id > arr[largest].id)
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r].id  > arr[largest].id)
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        userHeapify(arr, n, largest);
    }
}

// main function to do heap sort
void userHeapSort(tempobject arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        userHeapify(arr, n, i);
    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {

        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        userHeapify(arr, i, 0);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////


int main()
{


    tempobject* movie_rating_counts = new tempobject[user_array_size];
    tempobject* user_rating_counts = new tempobject[item_array_size];

    delete[] movie_rating_counts;
    delete[] user_rating_counts;



    clock_t startTime = clock();



    //userları koyacağımız storage ın tanımlanması
    storage<user> userlist = storage<user>(user_array_size);


    //Dosyayı okuma ve değerleri data structureda yerlerine yerleştirme
    string line;
    string userid;
    string itemid;
    string rating;
    ifstream myfile("C:\\Users\\ahmet\\Desktop\\2003 Proje\\csvler\\train.txt");
    int i = 0;

    if (myfile.is_open())
    {
        getline(myfile, line);
        while (getline(myfile, line))
        {
            stringstream ss(line);
            getline(ss, userid, ',');
            getline(ss, itemid, ',');
            getline(ss, rating, ',');
            
            userlist.add_to_user(stoi(userid), stoi(itemid), stof(rating));
//            std::cout << userlist.array[stoi(userid)].rating_array_class.array[stoi(itemid)].rate << std::endl;
            i++;
        }
        myfile.close();
    }
    else cout << "Unable to open file";

    clock_t endTime = clock();
    double elapsedTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    std::cout << "Elapsed time: " << elapsedTime << " seconds" << std::endl;
    
    //dosyadan okunup yerlerine yerleştirilmesi toplam 2.3 saniye sürüyor

    
    //array sort örneği, not küçükten büyüğe sıralıyor
    //userHeapSort(temparray, train_line_count);


    return 0;
}
