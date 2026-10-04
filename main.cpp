#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>

using namespace std;
using namespace chrono;

constexpr int DECK_SIZE = 9;
constexpr int TRIALS = 20;

struct Card {
    int a, b, c;

    bool operator==(const Card& other) const {
        return a == other.a &&
               b == other.b &&
               c == other.c;
    }
};

void printCard(const Card& c) {
    cout << "{" << c.a << ", " << c.b << ", " << c.c << "}";
}

// 실제 정답 판정용.
// 이 함수의 실행시간은 측정하지 않는다.
int findCard(const vector<Card>& deck, const Card& target) {
    for (int i = 0; i < DECK_SIZE; ++i) {
        if (deck[i] == target)
            return i;
    }

    return -1;
}

double mean(const vector<double>& v) {
    if (v.empty()) return 0.0;

    double sum = 0.0;

    for (double x : v)
        sum += x;

    return sum / v.size();
}

double median(vector<double> v) {
    if (v.empty()) return 0.0;

    sort(v.begin(), v.end());

    if (v.size() % 2 == 1)
        return v[v.size() / 2];

    return (v[v.size() / 2 - 1] +
            v[v.size() / 2]) / 2.0;
}

double standardDeviation(const vector<double>& v) {
    if (v.size() < 2)
        return 0.0;

    double m = mean(v);
    double sum = 0.0;

    for (double x : v)
        sum += (x - m) * (x - m);

    return sqrt(sum / (v.size() - 1));
}

void printStats(
    const string& name,
    const vector<double>& times
) {
    cout << "\n========== " << name << " ==========\n";

    if (times.empty()) {
        cout << "N = 0\n";
        return;
    }

    cout << fixed << setprecision(2);

    cout << "N        : " << times.size() << '\n';
    cout << "평균     : " << mean(times) << " ms\n";
    cout << "중앙값   : " << median(times) << " ms\n";
    cout << "표준편차 : "
         << standardDeviation(times)
         << " ms\n";

    cout << "최솟값   : "
         << *min_element(times.begin(), times.end())
         << " ms\n";

    cout << "최댓값   : "
         << *max_element(times.begin(), times.end())
         << " ms\n";
}

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ============================================
    // RANDOM
    // ============================================

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> valueDist(1, 3);

    // ============================================
    // 1. 9장 덱 자동 생성
    // ============================================

    vector<Card> deck;

    while ((int)deck.size() < DECK_SIZE) {

        Card c{
            valueDist(gen),
            valueDist(gen),
            valueDist(gen)
        };

        // 중복 카드 방지
        if (find(deck.begin(), deck.end(), c)
            == deck.end()) {

            deck.push_back(c);
        }
    }

    // ============================================
    // 2. 처음 화면
    // ============================================

    system("cls");

    cout << "========================================\n";
    cout << "          SET CARD SEARCH TEST\n";
    cout << "========================================\n\n";

    cout << "자동 생성된 9장 덱:\n\n";

    for (int i = 0; i < DECK_SIZE; ++i) {

        cout << setw(2) << i + 1 << ": ";

        printCard(deck[i]);

        cout << '\n';
    }

    cout << "\n========================================\n";
    cout << "20회 테스트를 곧 시작합니다.\n";
    cout << "========================================\n";

    cout.flush();

    // 화면을 볼 시간
    Sleep(1500);

    // ============================================
    // 3. YES 10개 / NO 10개
    // ============================================

    vector<bool> exists(TRIALS);

    for (int i = 0; i < 10; ++i)
        exists[i] = true;

    for (int i = 10; i < TRIALS; ++i)
        exists[i] = false;

    shuffle(exists.begin(), exists.end(), gen);

    // ============================================
    // 4. 통계용
    // ============================================

    vector<double> allTimes;
    vector<double> yesTimes;
    vector<double> noTimes;

    // YES일 때 실제 몇 번째 카드를 찾았는지도 기록
    vector<double> positionTimes[DECK_SIZE];

    int correct = 0;

    // ============================================
    // 5. 20회 실험
    // ============================================

    for (int trial = 0; trial < TRIALS; ++trial) {

        bool actualExists = exists[trial];

        Card target;
        int targetPosition = -1;

        // ----------------------------------------
        // YES
        // ----------------------------------------

        if (actualExists) {

            uniform_int_distribution<int>
                posDist(0, DECK_SIZE - 1);

            targetPosition = posDist(gen);

            target = deck[targetPosition];
        }

        // ----------------------------------------
        // NO
        // ----------------------------------------

        else {

            do {

                target = {
                    valueDist(gen),
                    valueDist(gen),
                    valueDist(gen)
                };

            } while (findCard(deck, target) != -1);
        }

        // ----------------------------------------
        // 화면 출력
        // ----------------------------------------

        system("cls");

        cout << "========================================\n";
        cout << "             TRIAL "
             << trial + 1
             << " / "
             << TRIALS
             << '\n';
        cout << "========================================\n\n";

        cout << "DECK\n\n";

        for (int i = 0; i < DECK_SIZE; ++i) {

            cout << setw(2) << i + 1 << ": ";

            printCard(deck[i]);

            cout << '\n';
        }

        cout << "\n----------------------------------------\n";

        cout << "TARGET: ";

        printCard(target);

        cout << "\n\n";

        cout << "Y = 덱에 있음\n";
        cout << "N = 덱에 없음\n";

        cout.flush();

        // ========================================
        // ★ 실제 생체 반응시간 측정 시작
        // ========================================

        auto start = steady_clock::now();

        char key = _getch();

        auto end = steady_clock::now();

        double elapsed =
            duration<double, milli>(
                end - start
            ).count();

        key = toupper(
            static_cast<unsigned char>(key)
        );

        // ----------------------------------------
        // 잘못된 키
        // ----------------------------------------

        if (key != 'Y' && key != 'N') {

            cout << "\n\n잘못된 입력입니다.";

            Sleep(700);

            --trial;

            continue;
        }

        // ----------------------------------------
        // 정답 판정
        // ----------------------------------------

        bool userAnswer = (key == 'Y');

        bool isCorrect =
            (userAnswer == actualExists);

        if (isCorrect)
            ++correct;

        allTimes.push_back(elapsed);

        if (actualExists) {

            yesTimes.push_back(elapsed);

            positionTimes[targetPosition]
                .push_back(elapsed);

        } else {

            noTimes.push_back(elapsed);
        }

        // ----------------------------------------
        // 결과 표시
        // ----------------------------------------

        cout << "\n\n";

        cout << "입력 : "
             << key
             << '\n';

        cout << fixed << setprecision(2);

        cout << "시간 : "
             << elapsed
             << " ms\n";

        cout << "결과 : "
             << (isCorrect ? "CORRECT" : "WRONG")
             << '\n';

        if (actualExists) {

            cout << "실제 위치 : "
                 << targetPosition + 1
                 << '\n';
        }

        Sleep(500);
    }

    // ============================================
    // 6. 최종 결과
    // ============================================

    system("cls");

    cout << "========================================\n";
    cout << "              FINAL RESULT\n";
    cout << "========================================\n\n";

    cout << fixed << setprecision(2);

    cout << "정답 : "
         << correct
         << " / "
         << TRIALS
         << '\n';

    cout << "정확도 : "
         << 100.0 * correct / TRIALS
         << "%\n";

    printStats("ALL", allTimes);

    printStats("YES (존재)", yesTimes);

    printStats("NO (부재)", noTimes);

    // ============================================
    // 7. YES 위치별 분석
    // ============================================

    cout << "\n========================================\n";
    cout << "          POSITION ANALYSIS\n";
    cout << "========================================\n";

    for (int i = 0; i < DECK_SIZE; ++i) {

        cout << "\nPosition "
             << i + 1
             << " : ";

        if (positionTimes[i].empty()) {

            cout << "N = 0";

        } else {

            cout << "N = "
                 << positionTimes[i].size();

            cout << ", 평균 = "
                 << mean(positionTimes[i])
                 << " ms";
        }
    }

    cout << "\n\n========================================\n";
    cout << "실험 종료.\n";
    cout << "========================================\n";

    return 0;
}
