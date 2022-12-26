#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;
const int max_user = 35190;
const int max_item = 7335;


class tempobject {
public:
    int id;
    int count;

    tempobject() {
        id = 0;
        count = 0;
    };

    void set(int Id)
    {
        id = Id;
        count++;
    }
};



void tempheapify(tempobject arr[], int n, int i)
{
    int smallest = i; // Initialize smallest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is smaller than root
    if (l < n && arr[l].count < arr[smallest].count)
        smallest = l;

    // If right child is smaller than smallest so far
    if (r < n && arr[r].count  < arr[smallest].count)
        smallest = r;

    // If smallest is not root
    if (smallest != i) {
        swap(arr[i], arr[smallest]);

        // Recursively heapify the affected sub-tree
        tempheapify(arr, n, smallest);
    }
}

// main function to do heap sort
void tempheapSort(tempobject arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        tempheapify(arr, n, i);
    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {

        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        tempheapify(arr, i, 0);
    }
}



int main()
{

//  MAX IDS ,  TARIN.TXT LINE COUNT
    int max_user_id=0;
    int max_item_id=0;
    int line_count = 0;




    clock_t tic = clock();

    tempobject* user_rating_counts = new tempobject[max_user];
    tempobject* movie_rating_counts = new tempobject[max_item];


    string line;
    string userid;
    string itemid;
    string rating;
    ifstream train("C:\\Users\\ahmet\\Desktop\\2003 Proje\\csvler\\train.txt");
    getline(train, line);
    while (getline(train, line))
    {
        stringstream strstream(line);
        getline(strstream, userid, ',');
        getline(strstream, itemid, ',');
        getline(strstream, rating, ',');
        int user_id = stoi(userid);
        int item_id = stoi(itemid);

        //MAX USER, ITEM ID
        if (user_id > max_user_id)max_user_id=user_id;
        if (item_id > max_item_id)max_item_id=item_id;

        //TRAIN.TXT LINE COUNT
        user_rating_counts[user_id].set(user_id);
        movie_rating_counts[item_id].set(item_id);


        line_count++;
    }
    train.close();


    tempheapSort(user_rating_counts, max_user);
    tempheapSort(movie_rating_counts, max_item);







    cout<<"\n\nTOP 10 USERS: "<<endl;
    for (int a = 0;a < max_user; a++){
        std::cout <<"id: "<< user_rating_counts[a].id <<" count: "<<user_rating_counts[a].count << std::endl;
        if (a > 9)break;
    }

    cout<<"\n\nTOP 10 ITEMS:"<<endl;
    for (int a = 0;a < max_item; a++){
        std::cout <<"id: "<< movie_rating_counts[a].id <<" count: "<<movie_rating_counts[a].count << std::endl;
        if (a > 9)break;
    }

    clock_t toc = clock();

    cout<<"\nMax user id: " << max_user_id<<" Max item id: " << max_item_id<<" Train.txt line count: " << line_count<<endl;
    cout << "\nElapsed time: " << (double)(toc - tic) / CLOCKS_PER_SEC << " seconds" << std::endl;


    delete[] movie_rating_counts;
    delete[] user_rating_counts;

    return 0;
}
