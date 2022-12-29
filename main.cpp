#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <ctime>
using namespace std;

clock_t tic = clock();


const int user_array_size = 35190;
const int item_array_size = 7335;
const int train_line_count = 1123345;
const int top_user_count  = 130;





//okunan satırları sort etmek için
class temp_object {
public:
    int userid;
    int itemid;
    float rating;

    //arraye yerleşmiş olacak olan objeleri bu fonksiyon ile atayacağız
    void set(int Userid, int Itemid, float Rating)
    {
        userid = Userid;
        itemid = Itemid;
        rating = Rating;
    }

    temp_object() {
        userid = 0;
        itemid = 0;
        rating = 0;
    };

};

void temp_Heapify(temp_object arr[], int n, int i)
{
    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l].userid > arr[largest].userid)
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r].userid  > arr[largest].userid)
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        temp_Heapify(arr, n, largest);
    }
}
// main function to do heap sort
void min_temp_Heap_Sort(temp_object arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        temp_Heapify(arr, n, i);
    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {

        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        temp_Heapify(arr, i, 0);
    }
}



//userları ve userların içinde ratingleri depolayacağımız data structure
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
            array[last_object_index].next_object_index = id;
            last_object_index = id;
        }
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
/*
 * user listin içinde bulunacak userların objesi
içinde movielerin ratinglerini tutan bir obje
ve kullanıcının kaç rating yaptığını gösteren değişken var
rating eklerken rating alan filmin de movie_rating_counts arrayinde sayısını arttırıyor
 */
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



storage<user> userlist = storage<user>(user_array_size);




string file_string = "ID,Predicted\n";

class temp_compare_obj{
public:
    double similarity;
    int common;
    double rating;
    temp_compare_obj(){
        similarity = -1;
        common = -1;
        rating = -1;
    }
    void set(double sim, int com, double rat){
        similarity = sim;
        common = com;
        rating = rat;
    }
};





//for Manhattan

void min_pred_heapify(temp_compare_obj arr[], int n, int i)
{
    int largest = i; // Initialize largest as root Since we are using 0 based indexing
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l].similarity > arr[largest].similarity)
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r].similarity > arr[largest].similarity)
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        min_pred_heapify(arr, n, largest);
    }
}
void min_pred_heap_Sort(temp_compare_obj arr[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        min_pred_heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {
        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        min_pred_heapify(arr, i, 0);
    }
}






struct similarity_output {
    double similarity;
    int common;
};





//Manhattan hesaplama
struct similarity_output  calc_Manhattan(int question_userid,int current_userid,int* item_ids,int id_array_size){
    struct similarity_output output{2147483647,0};


    int common = 0;
    double num = 0;
    for (int i=0;i<id_array_size;i++){
        double c_rat = userlist.array[current_userid].rating_array_class.array[item_ids[i]].rate;
        if (c_rat!=-1){
            common++;
            double q_rat = userlist.array[question_userid].rating_array_class.array[item_ids[i]].rate;

            num += abs(c_rat - q_rat);
        }
    }
    if (common!=0) {
        output.similarity = num / (5 * common);
        output.common = common;
    }



    return output;
};





void set_next_id(int* userid, int itemid){
    int next_index = userlist.array[*userid].next_object_index;

    if (next_index == 0) *userid = -1;

    else if (next_index == itemid){
        *userid = next_index;
        set_next_id(userid, itemid);
    }
    else    *userid = next_index;

}

void predict(int qid, int userid, int itemid){
    //TAHMIN KISMI



    int item_ids[1633];

    int a = 0;
    int b = 0;
    //tahmin edilecek kullanıcının item idlerini toplamak
    while(true){
        if (userlist.array[userid].rating_array_class.array[a].rate != -1){
            item_ids[b] = a;
            b++;
        }
        if(userlist.array[userid].rating_array_class.array[a].next_object_index == 0)break;
        else a = userlist.array[userid].rating_array_class.array[a].next_object_index;
    }
    int id_arr_len = b;


    temp_compare_obj* results = new temp_compare_obj[user_array_size];

    a = 0;
    b = 0;
    //tüm userlar üzerinde gezinip filmi izleyenlerin benzerliğini depolama
    while(a!=-1){

        //baktığımız user aranan filmi izlemiş mi?
        double current_rating = userlist.array[a].rating_array_class.array[itemid].rate;
        if (current_rating != -1){



            //for Manhattan
            struct similarity_output output = calc_Manhattan(userid,a,item_ids,id_arr_len);


            results[b].set(output.similarity, output.common, current_rating);
            b++;
        }
        set_next_id(&a, itemid);
    }



    //for cosine
//    max_pred_Heap_Sort(results , b);

    //for Manhattan
    min_pred_heap_Sort(results,b);


    long double result=0;
    int common_total = 0;

    //ağırlıklı ortalama alma
    for (int i=0; i<b; i++){
        if (i >= top_user_count)break;
        result += results[i].rating * results[i].common * results[i].common;
        common_total += results[i].common * results[i].common;
    }
    result = result / common_total;

    file_string += to_string(qid);
    file_string+=",";
    file_string += to_string(result);
    file_string+="\n";


    delete[] results;
}





int main() {





    //userliste(mainds) eklemeden önce satırların geçici arreye koyulup sıralanması gerkiyor çünkü
    //main data structure da next_objecti, eklenen son objeye göre ayarlıyoruz yani userların sıralı
    //bir şekilde eklenmesi gerekiyor

    temp_object* temparray = new temp_object[train_line_count];


    string line;
    string userid;
    string itemid;
    string rating;
    ifstream train("C:\\Users\\ahmet\\Desktop\\2003 Proje\\csvler\\train.txt");
    int i = 0;

    getline(train, line);
    while (getline(train, line)) {
        stringstream strstream(line);
        getline(strstream, userid, ',');
        getline(strstream, itemid, ',');
        getline(strstream, rating, ',');

        temparray[i].set(stoi(userid), stoi(itemid), stof(rating));

        i++;
    }
    train.close();

    min_temp_Heap_Sort(temparray, i);


    for (int a = 0; a < i; a++) {
    userlist.add_to_user(temparray[a].userid,temparray[a].itemid,temparray[a].rating);
    }

    delete[] temparray;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ifstream test("C:\\Users\\ahmet\\Desktop\\2003 Proje\\csvler\\test.txt");
    string qid;
    getline(test, line);
    while (getline(test, line)) {
        stringstream strstream(line);
        getline(strstream, qid, ',');
        getline(strstream, userid, ',');
        getline(strstream, itemid, ',');




        predict(stoi(qid), stoi(userid), stoi(itemid));

    }
    train.close();


    std::ofstream out_file("C:\\Users\\ahmet\\Desktop\\2003 Proje\\csvler\\submission.csv");

    // Write the string to the file
    out_file << file_string;

    // Close the file
    out_file.close();






    clock_t toc = clock();
    std::cout << "Elapsed time: " << (double)(toc - tic) / CLOCKS_PER_SEC << " seconds" << std::endl;

    return 0;
}
