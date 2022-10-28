#include <cstdio>
#include <vector>
#include <list>
#include <ctime>

typedef std::vector<std::list<int>> UnweightedG;
typedef std::vector<std::list<std::pair<int, int>>> WeightedG;

void readDataGraph(UnweightedG &graphOld, int connectionsOld) {
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

int checkToplogy(const WeightedG &graphNew, const std::vector<bool> &choosenServers, int nodes) {

    int cost = 0, edgesTopology = 0;
    for(const auto &edges: graphNew) {
        for(const auto server: edges) {
            if(choosenServers[server.first]) {

                if(edgesTopology > nodes)
                    return -1;

                edgesTopology++;
                cost += server.second;
            }
        }
    }

    if(edgesTopology / 2 == nodes)
        return cost / 2;

    return -1;
}

////check toplogy here
//for(int server: choosenFast)
//choosenServers[fastServers[server]] = true;
//
//for(int server: choosenSlow)
//choosenServers[slowServers[server]] = true;

void chooseFastServers(const WeightedG &graphNew, const std::vector<int> &fastServers, int nodes) {
    std::vector<int>choosenFast(nodes);
    for(int i = 1; i < nodes; i++)
        choosenFast[i] = i;

    const int FAST_LIMIT (fastServers.size());
    int minCost = 1e3; //todo: set this to correct max value
    bool stopSearch = false;

    while (true) {
        std::vector<bool> choosenServers(graphNew.size());

        try {

            for(int server: choosenFast)
                choosenServers[fastServers[server]] = true;

            int cost (checkToplogy(graphNew, choosenServers, nodes));

            if(cost != -1)
                stopSearch = true;
            if(cost < minCost)
                cost = minCost;

            choosenFast = succesor(choosenFast, FAST_LIMIT);
        }
        catch (...) {
            break;
        }
    }
    if(stopSearch) {
        //todo: stop the search topology found print results
    }
    else {
        //todo: continue the search with slow servers
    }
}

void chooseFastSlowServers(const WeightedG &graphNew, const std::vector<int> &fastServers, const std::vector<int> &slowServers,int nodes) {
    const int SLOW_LIMIT (slowServers.size());
    const int FAST_LIMIT (fastServers.size());

    std::vector<int>choosenFast(nodes);
    for(int i = 1; i < nodes; i++)
        choosenFast[i] = i;
    std::vector<int>fastCache(choosenFast);

    std::vector<int>choosenSlow(1, 0);
    std::vector<int>slowCache(1, 0);

    int minCost = 1e3; //todo: set this to correct max value
    bool stopSearch = false;

    while (true) {

        while (true) {
            //check topology here
            std::vector<bool> choosenServers(graphNew.size());
            for(int server: choosenFast)
                choosenServers[fastServers[server]] = true;
            for(int server: choosenSlow)
                choosenServers[slowServers[server]] = true;

            int cost (checkToplogy(graphNew, choosenServers, nodes));

            if(cost != -1)
                stopSearch = true;
            if(cost < minCost)
                cost = minCost;

            try {
                choosenSlow = succesor(choosenSlow, SLOW_LIMIT);
            }
            catch (...) {
                break;
            }
        }

        try {
            choosenFast = succesor(choosenFast, FAST_LIMIT);
            choosenSlow = slowCache;
        }
        catch (...) {

            if(stopSearch)
                break;

            fastCache.erase(fastCache.end() - 1);
            slowCache.push_back(slowCache[slowCache.size() - 1] + 1);

            if(fastCache.empty())
                break;

            choosenFast = fastCache;
            choosenSlow = slowCache;
        }
    }

    //todo: return the results
}



void solution() {
    int serversOld, connectionsOld;
    scanf("%d %d", &serversOld, &connectionsOld);

    UnweightedG graphOld(serversOld, std::list<int>());
    readDataGraph(graphOld, connectionsOld);

    int serversNew, connectionsNew, fServers;
    scanf("%d %d %d", &serversNew, &connectionsNew, &fServers);

    std::vector<bool> fs(serversNew, false);
    std::vector<int> fastServers(fServers), slowServers(0);
    readDataFS(fs, fServers, fastServers, slowServers);

    WeightedG graphNew(serversNew, std::list<std::pair<int, int>>());
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
