#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>


using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::setw;

void suoliuok(int, int, int);
int ejimas(int, int, int, vector<vector<int>>, int);


int lenta[16][16][2];      //savo patogumui padariau globalu kintamaji nes tingiu kaskart padavineti i kiekviena f-ja

int ejimuBudai[8][2] = { {-2,1}, {-1,2}, {1,2}, {2,1}, {2,-1}, {1,-2}, {-1,-2}, {-2,-1} };

int main() {
    int n = 0, p = 0, q = 0;

    cout << "Iveskite n (lentos ploti/ilgi) (max 16)" << endl;
    cin >> n;
    try {
        if (n < 1 || n>16) {
            throw n;
        }
    }
    catch (int n) {
        cout << n << " yra netinkamas skaicius! Veskite is naujo" << endl;
        cin >> n;
    }

    cout << "Iveskite startine pozicija p ir q (eilute ir stulpelis (skaiciais, skaiciuojant nuo virsaus ir nuo kaires (1-n)))" << endl;
    cin >> p >> q;
    try {
        if (p < 1 || p>n || q < 1 || q > n) {
            throw(n);
        }
    }
    catch (int n) {
        cout << "Ivestas starto laukas yra netinkamas ! Veskite is naujo" << endl;
        cin >> p >> q;
    }


    //nuliname lenta, paskui uzpildysime zingsniais
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < 2; k++)
                lenta[i][j][k] = 0;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < 8; k++) {
                if (0 <= i + ejimuBudai[k][0] && i + ejimuBudai[k][0] < n && 0 <= j + ejimuBudai[k][1] && j + ejimuBudai[k][1] < n) {
                    lenta[i][j][1] += 1;    //kiekvienam laukeliui priskiriame kiek ejimu is jo galima padaryti
                }
            }
        }
    }

    
    auto start = std::chrono::high_resolution_clock::now();
    suoliuok(p, q, n);
    std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
    cout << "Marsruto radimas arba jo nebuvimo radimas uztruko: " << diff.count() << "s" << endl;


    cout << "      ";
    for (int i = 1; i <= n; i++) {
        cout << setw(2) << i << " ";
    }
    cout << "\n\n";

    for (int i = 0; i < n; i++)
    {
        cout << setw(2) << i + 1 << "    ";
        for (int j = 0; j < n; j++)
        {
            cout << setw(2) << lenta[i][j][0] << " ";
        }
        cout << endl;
    }
    
    system("pause");
}

//funkcija kaip pavyzdyje
void suoliuok(int p, int q, int n) {
    int k = 1;
    vector<vector<int>> buvo;
    buvo.reserve(256);

    for (int i = 0; i < 256; i++) {
        buvo.push_back(vector<int>());
    }

    int EjimuPozicijos[256][2]{}; //visu zingsniu poziciju vektorius (kol kas max yra n=16)

    EjimuPozicijos[0][0] = p;   //irasome pradine pozicija (eilute)
    EjimuPozicijos[0][1] = q;   //irasome pradine pozicija (stulpelis)


    lenta[p - 1][q - 1][0] = 1;    //pirmas zingsnis

    for (int i = 0; i < 8; i++) {
        if (0 <= p - 1 + ejimuBudai[i][0] && p - 1 + ejimuBudai[i][0] < n && 0 <= q - 1 + ejimuBudai[i][1] && q - 1 + ejimuBudai[i][1] < n) {
            lenta[p-1+ejimuBudai[i][0]][q-1+ejimuBudai[i][1]][1] -= 1;    //kiekvienam laukeliui priskiriame kiek ejimu is jo galima padaryti
        }
    }

    
    int eilEiti, stulpEiti; //kitas zingsnis skirtas lentos pildymui

    while (k > 0 && k < n * n) {
        int koksZingsnis = ejimas(p, q, n, buvo, k);
        if (koksZingsnis == 0) {
            lenta[p - 1][q - 1][0] = 0;
            EjimuPozicijos[k - 1][0] = 0;
            EjimuPozicijos[k - 1][1] = 0;

            for (int i = 0; i < 8; i++) {
                if (0 <= p-1+ejimuBudai[i][0] && p-1+ejimuBudai[i][0] < n && 0 <= q-1+ejimuBudai[i][1] &&
                    q - 1 + ejimuBudai[i][1] < n) {
                    lenta[p-1+ejimuBudai[i][0]][q-1+ejimuBudai[i][1]][1] += 1;
                }
            }

            if(k>1){
                p = EjimuPozicijos[k - 2][0];
                q = EjimuPozicijos[k - 2][1];
            }
            k = k - 1;
        }
        else {
            buvo[k-1].push_back(koksZingsnis);
            eilEiti = ejimuBudai[koksZingsnis - 1][0];
            stulpEiti = ejimuBudai[koksZingsnis - 1][1];
            p = p + eilEiti;
            q = q + stulpEiti;
            EjimuPozicijos[k][0] = p;
            EjimuPozicijos[k][1] = q;
            lenta[p - 1][q - 1][0] = k + 1;

            for (int i = 0; i < 8; i++) {
                if (0 <= p - 1 + ejimuBudai[i][0] && p - 1 + ejimuBudai[i][0] < n && 0 <= q - 1 + ejimuBudai[i][1] &&
                    q - 1 + ejimuBudai[i][1] < n) {
                    lenta[p - 1 + ejimuBudai[i][0]][q - 1 + ejimuBudai[i][1]][1] -= 1;
                }
            }

            k = k + 1;
        }
    }
    buvo.clear();
}

//funkcija, atsakinga uz kito zingsnio parinkima
int ejimas(int eil, int stulp, int n, vector<vector<int>>buvo, int k) {
    int geriausias = 0;     //kuris ejimas geriausias
    int geriausioRez = 9;   //to ejimo galimu zingsniu skaicius
    int zings = k-1;
    int tempRez = 0;
    for (int i = 0; i < 8; i++) {
        if (std::find(buvo[zings].begin(), buvo[zings].end(), i+1)==buvo[zings].end() && 0<=eil-1+ejimuBudai[i][0] &&
            eil-1+ejimuBudai[i][0]<n && 0<=stulp-1+ejimuBudai[i][1] && stulp-1+ejimuBudai[i][1]<n) {
            tempRez = lenta[eil - 1 + ejimuBudai[i][0]][stulp - 1 + ejimuBudai[i][1]][1];
            if (lenta[eil-1+ejimuBudai[i][0]][stulp-1+ejimuBudai[i][1]][0]==0 && tempRez<geriausioRez) {
                geriausioRez = tempRez;
                geriausias = i+1;
            }
        }
    }
    
    return geriausias;
}