//
// Created by michal on 12/12/18.
//

#include "OpenServerCommand.h"
vector<double> explode1(string &s, const char &c)  {
    string buff{""};
    vector<double> v;
    for (auto n:s) {
        if (n != c) {
            buff += n;
        } else {
            double num;
            num = stod(buff);
            v.push_back(num);
            buff = "";
        }
    }
    return v;
}
void* serverThread(void *arg) {
    ServerParams *params = (ServerParams *) arg;

    xmlTable["/instrumentation/airspeed-indicator/indicated-speed-kt"];
    xmlTable["/instrumentation/altimeter/indicated-altitude-ft"];
    xmlTable["/instrumentation/altimeter/pressure-alt-ft"];
    xmlTable["/instrumentation/attitude-indicator/indicated-pitch-deg"];
    xmlTable["/instrumentation/attitude-indicator/indicated-roll-deg"];
    xmlTable["/instrumentation/attitude-indicator/internal-pitch-deg"];
    xmlTable["/instrumentation/attitude-indicator/internal-roll-deg"];
    xmlTable["/instrumentation/encoder/indicated-altitude-ft"];
    xmlTable["/instrumentation/encoder/pressure-alt-ft"];
    xmlTable["/instrumentation/gps/indicated-altitude-ft"];
    xmlTable["/instrumentation/gps/indicated-ground-speed-kt"];
    xmlTable["/instrumentation/gps/indicated-vertical-speed</node>"];
    xmlTable["/instrumentation/heading-indicator/indicated-heading-deg"];
    xmlTable["/instrumentation/magnetic-compass/indicated-heading-deg"];
    xmlTable["/instrumentation/slip-skid-ball/indicated-slip-skid"];
    xmlTable["/instrumentation/turn-indicator/indicated-turn-rate"];
    xmlTable["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"];
    xmlTable["/controls/flight/aileron"];
    xmlTable["controls/flight/rudder"];
    xmlTable["controls/flight/flaps"];
    xmlTable["controls/engines/engine/throttle"];
    xmlTable["/engines/engine/rpm"];

    vector<double> ve;
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int n;
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(params->getPort());

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    cout << "connected" << endl;
    /* If connection is established then start communicating */
    while (true) {
        pthread_mutex_lock(&mutex);
        clock_t time_start;
        time_start = clock();

        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
        string buf = buffer;
        ve = explode1(buf, ',');
        int i = 0;
        for (auto it = xmlTable.begin(); it != xmlTable.end(); ++it) {
            it.operator*().second = ve.at(i);
            i++;
        }

        clock_t time_end;
        time_end = time_start + 10 * params->getRate() * CLOCKS_PER_SEC / 1000;
        pthread_mutex_unlock(&mutex);
        while (clock() < time_end) {}
    }

}
int OpenServerCommand::doCommand(vector<string> &x){
    int num0 = 0;
    int num1 = 0;

    try {
        num0 = stoi(x.at(0));
        num1 = stoi(x.at(1));
    } catch (invalid_argument &exception1){
        throw "illegal arguments";
    }
    serverParams->setPort(num0);
    serverParams->setRate(num1);
    pthread_t trid;
    pthread_create(&trid, nullptr, serverThread, &serverParams);
    pthread_join(trid, nullptr);
    return 2;

}




