#include <cstdio>
#include <vector>
#include <list>
#include <ctime>

void readDataGraph(std::vector<std::list<int>> &graphOld, int connectionsOld) {
    for(int i = 0; i < connectionsOld; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        graphOld[x].push_back(y);
        graphOld[y].push_back(x);
    }
}

void readDataFS(std::vector<bool> &fs, int fServers, std::vector<int> &fastServers, std::vector<int> &slowServers) {
    for(int i = 0; i < fServers; i++) {
        int x;
        scanf("%d", &x);
        fs[x] = true;

        int start = 0;
        if(!fastServers.empty())
            start = fastServers[fastServers.size() -1] + 1;

        fastServers[i] = x;
        for(int j = start; j < x; j++)
            slowServers.emplace_back(j);

    }
}

void readDataGraph2(std::vector<std::list<std::pair<int, int>>> &graphNew, int connectionsNew) {
    for(int i = 0; i < connectionsNew; i++) {
        int x, y, z;
        scanf("%d %d %d", &x, &y, &z);
        graphNew[x].emplace_back(y, z);
        graphNew[y].emplace_back(x, z);
    }
}

void chooseFastServers(const std::vector<std::list<int>> &graphOld, std::vector<std::list<std::pair<int, int>>> &graphNew, const std::vector<int> &fastServers, int nodes) {

}

std::vector<int> succesor(const std::vector<int> &T, int n) {
    std::vector<int> U(T);
    int i = T.size() - 1;
    while (i >= 1 && T[i] == n - T.size() + i) {
        i--;
    }

    for(int j = i; j < T.size(); j++) {
        int tmp = T[i] + 1 + j - i;
        if (tmp == n) throw std::exception();
        U[j] = tmp;
    }

    return U;
}

void solution() {
    int serversOld, connectionsOld;
    scanf("%d %d", &serversOld, &connectionsOld);

    std::vector<std::list<int>> graphOld(serversOld, std::list<int>());
    readDataGraph(graphOld, connectionsOld);

    int serversNew, connectionsNew, fServers;
    scanf("%d %d %d", &serversNew, &connectionsNew, &fServers);

    std::vector<bool> fs(serversNew, false);
    std::vector<int> fastServers(fServers), slowServers(0);
    readDataFS(fs, fServers, fastServers, slowServers);

    std::vector<std::list<std::pair<int, int>>> graphNew(serversNew, std::list<std::pair<int, int>>());
    readDataGraph2(graphNew, connectionsNew);
}

int main() {
//    clock_t start, end;
//    start = clock();
//
//    solution();
//
//    end = clock();
//    printf("\nExecution time: %f", double ((end - start) / 1000.));
    std::vector<int> y{0,1,2,3};
    std::vector<int> x{0,1,2,3};

    std::vector<int> a{0}, b{0};


    int n = 5;
    while (1){
        printf("fast: '");
        for(int el: x)
            printf("%d ", el);
        printf("'");
        printf("\n");
        while (1) {
            printf("slow: '");
            for(int el: a)
                printf("%d ", el);
            printf("'");
            try {
                a = succesor(a, n);
            }
            catch (...) {
                break;
            }
            printf("\t");
        }
        printf("\n###");

        try {
            x = succesor(x, n);
            a = b;
        }
        catch (...) {

            printf("\n*******************");


            y.erase(y.end() - 1);
            b.push_back(b[b.size() - 1] + 1);
            if(y.empty())
                break;
            x = y;
            a = b;
        }

        printf("\n");
    }


//        for(int el: succesor({0,1,2}, 5))
//            printf("%d ", el);
//
//        printf("\n");



    return 0;
}
