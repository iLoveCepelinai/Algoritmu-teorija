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

void suoliuok(int p, int q, int n);
int ejimas(int eil, int stulp, int n, int buvo);


int lenta[16][16];      //savo patogumui padariau globalu kintamaji nes tingiu kaskart padavineti i kiekviena f-ja

int ejimuBudai[8][2] = { {-2,1}, {-1,2}, {1,2}, {2,1}, {2,-1}, {1,-2}, {-1,-2}, {-2,-1} };

int main(){
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
            lenta[i][j] = 0;
        }
    }


    auto start = std::chrono::high_resolution_clock::now();
        suoliuok(p, q, n);
    std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
    cout << "Marsruto radimas arba jo nebuvimo radimas uztruko: " << diff.count() << "s" << endl;

    cout << "   ";
    for (int i = 1; i <= n; i++) {
        cout << setw(2) << i << " ";
    }
    cout << "\n";

    for (int i = 0; i < n; i++)
    {
        cout << setw(2) << i+1 << " ";
        for (int j = 0; j < n; j++)
        {
            cout << setw(2) << lenta[i][j] << " ";
        }
        cout << endl;
    }

    system("pause");
}    

//funkcija kaip pavyzdyje
void suoliuok(int p, int q, int n) {
    int k = 1;
    int buvo = 0;

    int EjimuPozicijos[256][2]; //visu zingsniu poziciju vektorius (kol kas max yra n=16)

    EjimuPozicijos[0][0] = p;   //irasome pradine pozicija (eilute)
    EjimuPozicijos[0][1] = q;   //irasome pradine pozicija (stulpelis)

    vector <int> zingsniai (n-1); // n-1, o ne n, nes starto langelis jau uzimtas, liko 63 ejimai
    std::fill(zingsniai.begin(), zingsniai.end(), 0); //dabar uzpildau 0, paskui vertes keisis tarp 1-8

    lenta[p - 1][q - 1] = 1;    //pirmas zingsnis

    int eilEiti, stulpEiti; //kitas zingsnis skirtas lentos pildymui

    while (k > 0 && k < n*n) {
        int koksZingsnis = ejimas(p, q, n, buvo);
        if (koksZingsnis == 0) {
            lenta[p - 1][q - 1] = 0;
            EjimuPozicijos[k-1][0] = 0;
            EjimuPozicijos[k-1][1] = 0;

            p = EjimuPozicijos[k - 2][0];
            q = EjimuPozicijos[k - 2][1];

            buvo = zingsniai.back();
            zingsniai.pop_back();

            k = k - 1;
        }
        else {
            buvo = 0;
            zingsniai.push_back(koksZingsnis);
            eilEiti = ejimuBudai[koksZingsnis - 1][0];
            stulpEiti = ejimuBudai[koksZingsnis - 1][1];
            p = p + eilEiti;
            q = q + stulpEiti;
            EjimuPozicijos[k][0] = p;
            EjimuPozicijos[k][1] = q;
            lenta[p - 1][q - 1] = k+1;
            k = k + 1;
        }
    }
    zingsniai.clear();
}

//funkcija, atsakinga uz kito zingsnio parinkima
int ejimas(int eil, int stulp, int n, int buvo) {
    //8 if'ai (8 budai eiti + eiti negalima)                                                //1 - startas, 2 - galas
    if (eil - 2 >= 1 && stulp + 1 <= n && lenta[eil-3][stulp] == 0 && buvo < 1) {           //02
        return 1;                                                                           //0
    }                                                                                       //1
    
    else if (eil - 1 >= 1 && stulp + 2 <= n && lenta[eil-2][stulp+1] == 0 && buvo < 2) {    //002
        return 2;                                                                           //1
    }

    else if (eil + 1 <= n && stulp + 2 <= n && lenta[eil][stulp+1] == 0 && buvo < 3) {      //1
        return 3;                                                                           //002
    }

    else if (eil + 2 <= n && stulp + 1 <= n && lenta[eil+1][stulp] == 0 && buvo < 4) {      //1
        return 4;                                                                           //0
    }                                                                                       //02

    else if (eil + 2 <= n && stulp - 1 >= 1 && lenta[eil+1][stulp-2] == 0 && buvo < 5) {    // 1
        return 5;                                                                           // 0
    }                                                                                       //20

    else if (eil + 1 <= n && stulp - 2 >= 1 && lenta[eil][stulp-3] == 0 && buvo < 6) {      //  1
        return 6;                                                                           //200
    }

    else if (eil - 1 >= 1 && stulp - 2 >= 1 && lenta[eil-2][stulp-3] == 0 && buvo < 7) {    //200
        return 7;                                                                           //  1
    }

    else if (eil - 2 >= 1 && stulp - 1 >= 1 && lenta[eil-3][stulp-2] == 0 && buvo < 8) {    //20
        return 8;                                                                           // 0
    }                                                                                       // 1

    else {
        return 0;
    }
}