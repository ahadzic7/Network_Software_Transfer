#include <cstdio>
#include <vector>
#include <list>
#include <ctime>
#include <algorithm>

typedef std::vector<std::list<int>> UnweightedG;
typedef std::vector<std::list<std::pair<int, int>>> WeightedG;

void print_choosen( const std::vector<int> &fastServers, const std::vector<int> &slowServers,  const std::vector<int> &choosenFast, const std::vector<int> &choosenSlow) {
    printf("Fast: ");
    for(int server: choosenFast){
        printf("%d ", fastServers[server]);
    }
    printf("\nSlow: ");
    for(int server: choosenSlow){
        printf("%d ", slowServers[server]);
    }
    printf("\n\n");
}

void readDataGraph(UnweightedG &graphOld, int connectionsOld, std::vector<int> &degreeHist) {
    for(int i = 0; i < connectionsOld; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        graphOld[x].push_back(y);
        graphOld[y].push_back(x);
        degreeHist[x]++;
        degreeHist[y]++;
    }
}

void readDataFS(int fServers, std::vector<int> &fastServers, std::vector<int> &slowServers, int nodes) {
//    for(int i = 0; i < fServers; i++) {
//        int x;
//        scanf("%d", &x);
//
//        int start = 0;
//        if(!fastServers.empty())
//            start = fastServers[fastServers.size() -1] + 1;
//
//        fastServers.emplace_back(x);
//        for(int j = start; j < x; j++)
//            slowServers.emplace_back(j);
//    }
//    if(fastServers.size() + slowServers.size() != nodes) {
//        for(int j = fastServers[fastServers.size() - 1] + 1; j < nodes; j++)
//            slowServers.emplace_back(j);
//    }
    for(int i = 0; i < fServers; i++) {
        int x;
        scanf("%d", &x);

        fastServers.emplace_back(x);
    }
    std::sort(fastServers.begin(), fastServers.end());

    int start = fastServers[0];
    if(start != 0) {
        for(int j = 0; j < start; j++)
            slowServers.emplace_back(j);
    }



    for(int i = 0; i < fastServers.size() - 1; i++) {
        start = fastServers[i] + 1;
        int end = fastServers[i + 1];
        for(int j = start; j < end; j++)
            slowServers.emplace_back(j);
    }

    if(fastServers.size() + slowServers.size() != nodes) {
        for(int j = fastServers[fastServers.size() - 1] + 1; j < nodes; j++)
            slowServers.emplace_back(j);
    }

}

void readDataGraph2(WeightedG &graphNew, int connectionsNew) {
    for(int i = 0; i < connectionsNew; i++) {
        int x, y, z;
        scanf("%d %d %d", &x, &y, &z);
        graphNew[x].emplace_back(y, z);
        graphNew[y].emplace_back(x, z);
    }
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

int checkToplogy(const WeightedG &graphNew, const std::vector<bool> &choosenServers, int links, std::vector<int> &degreeHistOld) {

    int cost = 0, edgesTopology = 0;
    std::vector<int> degreeHist(graphNew.size(), 0);

    for(int i = 0; i < graphNew.size(); i++) {
        if(choosenServers[i]) {
            for(const auto server: graphNew[i]) {
                if(choosenServers[server.first]) {
                    if(edgesTopology / 2 > links) {
                        return -1;
                    }
                    degreeHist[server.first]++;
                    edgesTopology++;
                    cost += server.second;
                }
            }
        }
    }

    if(edgesTopology / 2 == links) {
        std::sort(degreeHist.begin(), degreeHist.end(), [](int x, int y) {return x > y;});
        std::sort(degreeHistOld.begin(), degreeHistOld.end(), [](int x, int y) {return x > y;});
        for(int i = 0; i < degreeHistOld.size(); i++)
            if(degreeHist[i] != degreeHistOld[i])
                return -1;

        return cost / 2;
    }


    return -1;
}


int chooseFastServers(const WeightedG &graphNew, const std::vector<int> &fastServers, int nodes,  std::vector<int> &degreeHistOld) {
    std::vector<int>choosenFast(nodes);
    for(int i = 1; i < nodes; i++)
        choosenFast[i] = i;

    const int FAST_LIMIT (fastServers.size());
    int minCost = 1e9; //todo: set this to correct max value
    bool stopSearch = false;

    while (true) {
        std::vector<bool> choosenServers(graphNew.size());

        try {

            for(int server: choosenFast)
                choosenServers[fastServers[server]] = true;

            int cost (checkToplogy(graphNew, choosenServers, nodes, degreeHistOld));

            if(cost != -1)
                stopSearch = true;
            if(cost < minCost)
                minCost = cost;

            choosenFast = succesor(choosenFast, FAST_LIMIT);
        }
        catch (...) {
            break;
        }
    }
    if(stopSearch)
        return minCost;

    return -1;
}


std::pair<int, int> chooseFastSlowServers(const WeightedG &graphNew, const std::vector<int> &fastServers, const std::vector<int> &slowServers,int nodes, int links, std::vector<int> &degreeHistOld) {
    const int SLOW_LIMIT (slowServers.size());
    const int FAST_LIMIT (fastServers.size());

    std::vector<int>choosenFast(FAST_LIMIT);
    for(int i = 1; i < FAST_LIMIT; i++)
        choosenFast[i] = i;
    std::vector<int>fastCache(choosenFast);

    int size = std::abs(nodes - FAST_LIMIT);
    std::vector<int>choosenSlow(size, 0);
    for(int i = 1; i < size; i++)
        choosenSlow[i] = i;
    std::vector<int>slowCache(choosenSlow);

    int minCost = 1e9; //todo: set this to correct max value
    bool stopSearch = false;

    while (true) {

        while (true) {
            //check topology here
            std::vector<bool> choosenServers(graphNew.size());
            for(int server: choosenFast)
                choosenServers[fastServers[server]] = true;
            for(int server: choosenSlow)
                choosenServers[slowServers[server]] = true;

            //print_choosen(fastServers, slowServers, choosenFast, choosenSlow);
            //printf("%d %d\n", choosenFast.size(), choosenSlow.size());
            int cost (checkToplogy(graphNew, choosenServers, links, degreeHistOld));
            if(cost != -1 && cost < minCost) {
                //printf("%d %d\n", choosenFast.size(), choosenSlow.size());
                print_choosen(fastServers, slowServers, choosenFast, choosenSlow);
                minCost = cost;
                stopSearch = true;
            }
            if(!choosenSlow.empty()) {
                try {
                    choosenSlow = succesor(choosenSlow, SLOW_LIMIT);
                }
                catch (...) {
                    break;
                }
            }
            else
                break;
        }

        try {
            choosenFast = succesor(choosenFast, FAST_LIMIT);
            choosenSlow = slowCache;
        }
        catch (...) {

            if(stopSearch)
                break;

            fastCache.erase(fastCache.end() - 1);

            if(!slowCache.empty()) {
                slowCache.push_back(slowCache[slowCache.size() - 1] + 1);
            } else {
                slowCache.push_back(0);
            }

            if(fastCache.empty())
                break;

            choosenFast = fastCache;
            choosenSlow = slowCache;
        }
    }

    if(stopSearch) {
        return std::make_pair(minCost, fastCache.size());
    }
    return std::make_pair(-1, -1);
}



void findMinTopology(const WeightedG &graphNew, int nodes, int links, const std::vector<int> &fastServers, const std::vector<int> &slowServers,  std::vector<int> &degreeHistOld) {
    if(nodes < fastServers.size()) {
//        int minCost = chooseFastServers(graphNew, fastServers, nodes, degreeHistOld);
//        if(minCost != -1) {
//            printf("a)Fast servers %d, min cost %d %d\n", minCost, fastServers.size());
//            return;
//        }
//        return;
    }

    auto result(chooseFastSlowServers(graphNew, fastServers, slowServers, nodes, links, degreeHistOld));

    if(result.first != -1)
        printf("b)Fast servers %d, min cost %d\n", result.second, result.first);
    else
        printf("No topology was found!\n");
}

void solution() {
    int serversOld, connectionsOld;
    scanf("%d %d", &serversOld, &connectionsOld);

    UnweightedG graphOld(serversOld, std::list<int>());
    std::vector<int> degreeHist(serversOld, 0);
    readDataGraph(graphOld, connectionsOld, degreeHist);

    int serversNew, connectionsNew, fServers;
    scanf("%d %d %d", &serversNew, &connectionsNew, &fServers);

    std::vector<bool> fs(serversNew, false);
    std::vector<int> fastServers(0), slowServers(0);
    readDataFS(fServers, fastServers, slowServers, serversNew);

    WeightedG graphNew(serversNew, std::list<std::pair<int, int>>());
    readDataGraph2(graphNew, connectionsNew);

    findMinTopology(graphNew, serversOld, connectionsOld, fastServers, slowServers, degreeHist);
//    printf("%d", checkToplogy(graphNew, {true,true,true,false,false,true,false,true,true,false,true,true,false,true,true}, connectionsOld, degreeHist));
//    printf("%d", checkToplogy(graphNew, {false,true,true,false,false,false,true,true,true,true,false,false,false,true,true}, connectionsOld, degreeHist));
//    printf("%d", checkToplogy(graphNew, {false, false,false,false,true,true,true,false, true,true}, connectionsOld, degreeHist));
//    printf("%d", checkToplogy(graphNew, {false, true,true,false,true,false,true,false, false,true}, connectionsOld, degreeHist));
}

void testSuccesor() {
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



}

int main() {
    clock_t start, end;
    start = clock();

    solution();

    end = clock();
    printf("\nExecution time: %f", double ((end - start) / 1000.));

    return 0;
}
