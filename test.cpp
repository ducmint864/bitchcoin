#include <iostream>
#include <vector>
#include <utility>

using namespace std;



void sortVec(vector<pair<int, short>>& vec)
{
    int i, j, minIndex;
    pair<int, short> tmp;
    for (i = 0; i < vec.size() - 1; i++)
    {

        if (vec[i].first % 2 == 0) {vec.erase(vec.begin() + i); continue;}

        minIndex = i;
        for (j = i+1; j < vec.size(); j++)
        {
            if (vec[j].first < vec[minIndex].first) 
            {
                minIndex = j;

                // swapping
                tmp = vec[minIndex]; 
                vec[minIndex] = vec[i];
                vec[i] = tmp;
            }
        }

    }
}

int main()
{
    vector<pair<int, short>> array;
    int tmp;
    array.reserve(10);

    for (int i = 0; i < 10; i++)
    {
        cin >> tmp; cin.ignore();
        // <value, count>
        pair<int, short> element(tmp, 0);
        array.push_back(element);
    }

    sortVec(array);

    int  i, j;
    pair<int, short> max(0, -1);
    
    for (i = 0; i < array.size()-1; i++)
    {
        j = i + 1;
        while (array[i].first == array[j].first)
        {
            array[i].second++;
            array.erase(array.begin() + j);
        }

        if (array[i].second > max.second) {max = array[i];}

    }

    cout << "So le xuat hien nhieu lan nhat : " << max.first << endl;
    cout << "So lan xuat hien : " << max.second << endl;
    
    return 0;
}