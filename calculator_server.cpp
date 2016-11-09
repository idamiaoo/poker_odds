#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include <pokerstove/penum/ShowdownEnumerator.h>
#include <grpc++/grpc++.h>
#include "calculator.pb.h"
#include "calculator.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using calculator::Request;
using calculator::Reply;
using calculator::Calculator;

using namespace pokerstove;

class CalculatorServiceImpl final : public Calculator::Service {
    Status Calc(ServerContext* context, const Request* request,
                  Reply* reply) override {
  /*
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
   */
    int i,j;
    int game = request->game();
    int hand = request->pokers_size();
    const ::google::protobuf::Map< ::google::protobuf::int32, ::std::string > pokers = request->pokers();
    ::google::protobuf::Map< ::google::protobuf::int32, ::std::string >::const_iterator it;
    std::vector<CardDistribution> hands(hand - 1);
    for (i = 1; i < hand; i++){

      it = pokers.find(i);
      if (it == pokers.end()){
        continue;
      }
      std::cout << it->second << std::endl;
      hands[i-1].parse(it->second);
    }
    ShowdownEnumerator showdown;
    std::vector<EquityResult> result = showdown.calculateEquity(
      hands,
      CardSet(pokers.find(0)->second),
      PokerHandEvaluator::alloc("h")
    );
    ::google::protobuf::Map< ::google::protobuf::int32, double >* winpots = reply->mutable_winpots();
    ::google::protobuf::Map< ::google::protobuf::int32, ::google::protobuf::int32 >* wintype = reply->mutable_wintype(); 
    for (i = 0; i < result.size(); i ++){
      std::cout << i << ":" << result.at(i).winShares +result.at(i).tieShares << std::endl;	
      std::cout << "111111111" << std::endl;
      winpots->insert(::google::protobuf::Map< ::google::protobuf::int32, double >::value_type(i,result.at(i).winShares +result.at(i).tieShares));
      if (i != 0) {
        for (j = 0; j < result.at(0).winEval.size(); j++){
	  std:: cout << j << ":" << result.at(i).winEval.at(j) << std::endl;
          result.at(0).winEval[j] += result.at(i).winEval.at(j);   
        }
        std::cout << "222222222" << std::endl;
      }
    }
    	
    for (i = 0, j = 0; j < result.at(0).winEval.size(); j++){
      if (j == 2 || j == 3 || j == 6) {
        continue;
      }
      if ( j < 2) {
        i = j;
      } else if (j > 3 && j < 6){
        i = j - 2;  
      } else {
        i = j - 3;
      } 
      std::cout << i << ":" << result.at(0).winEval.at(j) << std::endl;
      std::cout << "333333333" << std::endl;
      wintype->insert(::google::protobuf::Map< ::google::protobuf::int32, ::google::protobuf::int32 >::value_type(i,result.at(0).winEval.at(j)));
    }
   
    reply->set_game(game);
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  CalculatorServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
