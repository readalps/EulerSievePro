#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>

typedef unsigned char u8;
//typedef uint64_t ulong;
typedef unsigned long ulong;
static std::vector<ulong> s_vecPrime;

void showDetails()
{
    ulong pos = 0;
    while (true) {
        if (pos >= s_vecPrime.size())
            break;
        std::cout << s_vecPrime[pos++];
        if (pos < s_vecPrime.size())
            std::cout << ",";
        if (pos % 500 == 0) {
            std::cout << std::endl << " " << pos << " primes listed, "
                << s_vecPrime.size() - pos << " left to show [c] continue; <q> quit: ";
            std::string strInput;
            getline(std::cin, strInput);
            if (strInput == "q")
                break;
            std::cout << std::endl;
        }
        if (pos % 100 == 0)
            std::cout << std::endl;
    }
}

bool eulerSievePro(ulong raw_last)
{
    DWORD tickBegin = GetTickCount();
    ulong last = raw_last / 2;
    u8* pOdd = new u8[last];
    if (!pOdd) {
        printf("Lack of memory.\n");
        return false;
    }

    ulong sum = 1;
    ulong uplimit = 0;
    s_vecPrime.push_back(2);
    memset(pOdd, 1, last);
    for (ulong halfIdx = 1; halfIdx < last; ++halfIdx) {
        ulong num = (halfIdx + halfIdx) + 1;
        if (pOdd[halfIdx] == 1) {
            ++sum;
            s_vecPrime.push_back(num);
        }
        for (ulong idx = 1; idx < sum; ++idx) {
            if (uplimit != 0 && idx >= uplimit)
                break;
            ulong prime = s_vecPrime[idx];
            ulong multiple = num * prime;
            if (multiple >= raw_last) {
                uplimit = idx;
                break;
            }
            pOdd[multiple / 2] = 0;
            if (num % prime == 0)
                break;
        }
    }
    std::cout <<" " << sum << " primes found in " << GetTickCount() - tickBegin << " milliseconds.\n\n";
    delete []pOdd;
    return true;
}

bool str2num(const std::string& str, ulong& val)
{
    if (8 == sizeof(ulong)) {
        if (str > "8589934592") {
            printf("\n Invalid input - the biggest number could be 2^33.\n");
            return false;
        }
    }
    else {
        if (str >= "4294967296") {
            printf("\n Invalid input - the biggest number could be 2^32 - 1.\n");
            return false;
        }
    }
    size_t len = str.length();
    val = 0;
    for (size_t idx = 0; idx < len; ++idx) {
        char ch = str[idx];
        if (ch > '9' || ch < '0') {
            printf("\n Invalid input - with non-numeric character.\n");
            return false;
        }
        val = val * 10 + (ch - '0');
    }
    if (val <= 2) {
        printf("\n Invalid input - at least 3.\n");
        return false;
    }
    return true;
}

int main()
{
    printf(" EulerSievePro: a method to find out all primes below the number that you specify here please: ");
    std::string strInput;
    getline(std::cin, strInput);
    ulong raw_last = 0;
    if (!str2num(strInput, raw_last))
        return 0;
    printf("\n Only the sum of all primes needed [y/n](y as default): ");
    getline(std::cin, strInput);
    bool bDetail = (strInput == "n");
    if (bDetail)
        std::cout << std::endl << " Start to work out all primes below " << raw_last << "...\n";
    else
        std::cout << std::endl << " Start to work out the sum of all primes below " << raw_last << "...\n";
    if (!eulerSievePro(raw_last))
        return 0;
    if (bDetail)
        showDetails();
    return 0;
}
