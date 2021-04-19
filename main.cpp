//references: Sorting-2.pdf, Sorting-3.pdf, https://www.asciiart.eu/miscellaneous/money (See “Quarter” by Joan G. Stark), http://www.asciiworld.com/-Eagles-.html
//IMDB database: https://datasets.imdbws.com
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_map>
using namespace std;
typedef chrono::high_resolution_clock Clock;

struct film
{
    string title;
    string year;
    string runtime;
    string genres;
    string primaryGenre;
    film(string &_title, string &_year, string &_runtime, string &_genres)
    {
        title = _title;
        year = _year;
        runtime = _runtime;
        genres = _genres;
    };
};
int loadFilms(film* _films[])
{ int numFilms = 0;
    ifstream file("justmovies.tsv");
    if (file.is_open())
    {
        string header; //we don't need the header
        getline(file, header);

        for(int i = 0; i < 183777; i++)
        {
            string tconst, titleType, primaryTitle, originalTitle, isAdult, startYear, endYear, runtimeMinutes, genres;
            getline(file, tconst, '\t');
            getline(file, titleType, '\t');
            getline(file, primaryTitle, '\t');
            getline(file, originalTitle, '\t');
            getline(file, isAdult, '\t');
            getline(file, startYear, '\t');
            getline(file, endYear, '\t');
            getline(file, runtimeMinutes, '\t');
            getline(file, genres,'\r');

            //deal with parenthesis
            if(genres[0] == '\"')
            {
                string newString = genres.substr(1, genres.size() - 2);
                genres = newString;
            }
            string makeSure = genres.substr(0, 5);
            if(!stoi(isAdult) && makeSure != "Adult") //exclude adult titles
            {
                film* newFilm = new film(originalTitle, startYear, runtimeMinutes, genres);
                newFilm->primaryGenre = genres.substr(0, genres.find(','));
                _films[numFilms] = newFilm;
                numFilms++;
            }
        }
    }
    else
    {
        cout << "error: file not open.";
        return 0;
    }
    file.close();
    return numFilms;
}
void printHeads()
{
    cout << "        _.-'~~`~~'-._\n"
            "     .'`  B   E   R  `'.\n"
            "    / I               T \\\n"
            "  /`       .-'~\"-.       `\\\n"
            " ; L      / `-    \\      Y ;\n"
            ";        />  `.  -.|        ;\n"
            "|       /_     '-.__)       |\n"
            "|        |-  _.' \\ |        |\n"
            ";        `~~;     \\\\        ;\n"
            " ;  INGODWE /      \\\\)P    ;\n"
            "  \\  TRUST '.___.-'`\"     /\n"
            "   `\\                   /`\n"
            "     '._   1 9 9 7   _.'\n"
            " jgs    `'-..,,,..-'`\n";
}
void printTails()
{
    cout << "        _.-'~~`~~'-._\n"
            "     .'` T E D S T A`'.\n"
            "    /  I             T  \\\n"
            "  /` N    E PLURIBUS   E `\\\n"
            " ; U        UNUM         S ;\n"
            ";\\=======-. ,~\\  .-=======/ ;\n"
            "|  \\=======\\_||_/=======/   |\n"
            "|     \\\\=====##=====//      |\n"
            ";           \\##/            ;\n"
            " ;   ~~     /||\\      ~~   ;\n"
            "  \\    \\~~~~~~~~~~~~~/    /\n"
            "   `\\  QUARTER DOLLAR   /`\n"
            "     '._             _.'\n"
            "        `'-..,,,..-'`\n";
}
//sorting algorithms
void merge(film* arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    film* X[n1];
    film* Y[n2];

    for(int i = 0; i < n1; i++)
    {
        X[i] = arr[left + i];
    }
    for(int j = 0; j < n2; j++)
    {
        Y[j] = arr[mid + 1 + j];
    }

    //merge the arrays X and Y into arr
    int i = 0;
    int j = 0;
    int k = left;

    while(i < n1 && j < n2)
    {
        //if(X[i]->genres <= Y[j]->genres)
        if(X[i]->genres.compare(Y[j]->genres) < 0)
        {
            arr[k] = X[i];
            i++;
        }
        else
        {
            arr[k] = Y[j];
            j++;
        }
        k++;
    }
    //when we run out of elements in either X or Y append the remaining elements
    while(i < n1)
    {
        arr[k] = X[i];
        i++;
        k++;
    }

    while(j < n2)
    {
        arr[k] = Y[j];
        j++;
        k++;
    }

}
void mergeSort(film* arr[], int left, int right)
{
    if(left < right)
    {
        int middle = left + ((right - left) / 2);

        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        merge(arr, left, middle, right); //merge sorted subarrays
    }
}
void shellSort(film* arr[], int n)
{
    for (int gap = n / 2; gap > 0; gap /=2)
    {
        for(int i = gap; i < n; i +=1)
        {
            film* temp = arr[i];
            int j;
            //for(j = i; j >= gap && arr[j - gap]->genres > temp->genres; j -= gap)
            for(j = i; j >= gap && arr[j - gap]->genres.compare(temp->genres) > 0; j -= gap)
            {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}
int partition(film* array[], int low, int high)
{
    //select the pivot element
    film* pivot = array[low];
    //film* pivot = array[rand() % high];
    int up = low;
    int down = high;
    while(up < down)
    {
        for(int j = up; j < high; j++)
        {
            //if(array[up]->genres > pivot->genres)
            if(array[up]->genres.compare(pivot->genres) > 0)
            {
                break;
            }
            up++;
        }
        for(int j = high; j > low; j--)
        {
            //if(array[down]->genres < pivot->genres)
            if(array[down]->genres.compare(pivot->genres) < 0)
            {
                break;
            }
            down--;
        }
        if(up < down)
        {
            film* temp = array[up];
            array[up] = array[down];
            array[down] = temp;

            //swap(array[up], array[down]);
        }
    }
    film* temp = array[low];
    array[low] = array[down];
    array[down] = temp;
    //swap(array[low], array[down]);
    return down;
}
void quickSort(film* array[], int low, int high)
{
    if(low < high)
    {
        int pivot = partition(array, low, high);
        quickSort(array, low, pivot - 1);
        quickSort(array, pivot + 1, high);
    }
}

int main()
{
    int index = 1;
    film* films[177826];

    //print main menu
    while(index != 0)
    {
        cout << "\nRandom Decision Generator\n"
                " Main Menu:\n"
                "0 - Exit\n"
                "1 - Flip a Coin\n"
                "2 - Custom Decision\n"
                "3 - Choose a random film title\n"
                "(choose an index 0-3)\n";
        cin >> index;
        switch (index)
        {
            case 0: //exit case
            {
                continue;
            }
            case 1: //flip a coin
            {
                int coinFlip = int(random()) % 2;
                if(coinFlip == 1)
                {
                    printHeads();
                    cout << "            Heads!\n";
                }
                if(coinFlip == 0)
                {
                    printTails();
                    cout << "            Tails!\n";
                }
                break;
            }
            case 2: // enter your choices
            {
                cout << "Custom Decision:\n Enter your options:\n";
                cout << "(type \"choose\" to make a choice)\n";
                string option;
                vector<string> options;
                while (option != "choose") {
                    getline(cin, option);
                    if (option == "choose") { continue; }
                    options.push_back(option);
                }
                cout << "Random Arbitrary Decision:\n" << options[int(random()) % options.size()] << "\n";
                break;
            }
            case 3: //choose a film but sort first
            {
                int numFilms = loadFilms(films); //first load the film from the IMDB file
                if (numFilms > 0)
                {
                    cout << "Success: " << numFilms << " films loaded.\n";
                }
                else
                {
                    cout << "Loading failed: something went wrong.\n";
                    continue;
                }
                chrono::time_point<chrono::steady_clock> t1; //synchronise your watches
                chrono::time_point<chrono::steady_clock> t2;

                cout << "Choose a Film: Sort by Genre\n"
                        "How would you like to sort? "
                        "(Type the Name of the Sorting Algorithm)\n"
                        "Merge Sort\n"
                        "Shell Sort\n"
                        "Quick Sort\n";

                string sortChoice;
                bool goodChoice = false;
                while (!goodChoice) // choose a sorting algorithm
                {
                    getline(cin, sortChoice);
                    if (sortChoice == "Merge Sort" || sortChoice == "merge sort")
                    {
                        t1 = Clock::now();
                        mergeSort(films, 0, numFilms - 1);
                        t2 = Clock::now();
                        goodChoice = true;
                    }
                    else if(sortChoice == "Shell Sort" || sortChoice == "shell sort")
                    {
                        t1 = Clock::now();
                        shellSort(films, numFilms);
                        t2 = Clock::now();
                        goodChoice = true;
                    }
                    else if(sortChoice == "Quick Sort" || sortChoice == "quick sort")
                    {
                        t1 = Clock::now();
                        quickSort(films, 0, numFilms - 1);
                        t2 = Clock::now();
                        goodChoice = true;
                    }
                    else //invalid input
                    {
                        if (!sortChoice.empty())
                        {
                            cout << "Invalid Input: Try again.\n";
                        }
                        continue;
                    }
                }
                goodChoice = false;

                string choice = "genre";
                unordered_map<string, pair<int, int>> genreRangeIndices;
                while (!goodChoice) // choose a genre
                {

                    if(choice == "menu")
                    {
                        goodChoice = true;
                        continue;
                    }
                    else if(choice == "subgenre" || choice == "genre")
                    {
                        int begin = 0, end;
                        string prevGenres, currentGenres;
                        if(choice == "subgenre")
                        {
                            prevGenres = films[0]->genres;
                        }
                        else
                        {
                            prevGenres = films[0]-> primaryGenre;
                        }
                        for(int i = 0; i < numFilms; i++)
                        {
                            if(choice == "subgenre")
                            {
                                currentGenres = films[i]->genres;
                            }
                            else
                            {
                                currentGenres = films[i]->primaryGenre;
                            }

                            if(prevGenres != currentGenres)// || i == numFilms - 1)
                            {
                                end = i;
                                cout << prevGenres << "....." << end - begin << " titles\n";
                                pair<int, int> pear;
                                pear.first = begin;
                                pear.second = end;
                                genreRangeIndices.emplace(prevGenres, pear);
                                prevGenres = currentGenres;
                                begin = end;
                            }
                        }
                        cout << "\nResults: \nItems Sorted: " << numFilms << "\n";
                        double time = chrono::duration_cast<chrono::nanoseconds> (t2 - t1).count();
                        cout << "Time Elapsed (ns): " << chrono::duration_cast<chrono::nanoseconds> (t2 - t1).count() << "\n";
                        cout << "Time Elapsed (s): " << time / 1000000000 << "\n";
                        cout << "\nSelect a genre by typing its name.\n"
                                "(Enter \"menu\" to return to the main menu)\n"
                                "(Enter \"genre\" to display by primary genre or \"subgenre\" to display subgenres):\n";
                    }
                    else if(genreRangeIndices.count(choice))
                    {
                        int diff = genreRangeIndices[choice].second - genreRangeIndices[choice].first;
                        int randomIndex = (int(random()) % diff) + genreRangeIndices[choice].first;
                        film* chosenFilm = films[randomIndex];
                        cout << "Random Movie: \n"
                                "Title: " << chosenFilm->title << "\n";
                        cout << "Primary Genre: " << chosenFilm->primaryGenre << "\n";
                        cout << "Genres: " << chosenFilm->genres << "\n";
                        cout << "Runtime (minutes): " << chosenFilm->runtime << "\n";
                        cout << "Year: " << chosenFilm->year << "\n";
                        cout << "\nSelect a genre by typing its name.\n"
                                "(Enter \"menu\" to return to the main menu)\n"
                                "(Enter \"genre\" to display by primary genre or \"subgenre\" to display subgenres):\n";
                    }
                    else
                    {
                        cout << "Invalid input: try again.\n";
                        cout << "\nSelect a genre by typing its name.\n"
                                "(Enter \"menu\" to return to the main menu)\n"
                                "(Enter \"genre\" to display by primary genre or \"subgenre\" to display subgenres):\n";
                    }
                    cin >> choice;
                }
                break;
            }
            default:
            {
                cout << "invalid index: try again.\n";
                break;
            }
        }
    }
    return 0;
}
