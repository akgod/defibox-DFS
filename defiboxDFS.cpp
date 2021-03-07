
#include <string>
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <math.h>

using namespace eosio;
using namespace std;

#define EOS_CONTRACT "eosio.token"_n
#define BOX_CONTRACT "swap.defi"_n
#define DFS_CONTRACT "defisswapcnt"_n

#define EOS_SYMBOL symbol("EOS",4)
#define BOX_SYMBOL symbol("BOX",6)
#define DFS_SYMBOL symbol("DFS",4)


struct token {
	name   contrat_name;
  symbol token_symbol;
};

struct ndx_symbol {
	name   contrat_name;
  symbol token_symbol;
};

class [[eosio::contract("trademine")]] trademine : public eosio::contract {

  public:     
    using contract::contract;

    [[eosio::action]]
    void notify(name user, std::string msg) {
      require_auth(get_self());
      require_recipient(user);
    }


    [[eosio::action]] 
    void boxmine(int64_t boxpoolid,int64_t time) {                        
      require_auth(get_self());  
      boxpool_index boxpools( get_self(), get_first_receiver().value );     
      auto itr = boxpools.find(boxpoolid);
      check(itr != boxpools.end(),"boxpools have not this boxpoolid" );
                        
            
      auto order_amount = (uint64_t)10000;       

              action( 
                  permission_level{ get_self(), name("active") },
                  get_self(), name("eosasset"),
                  std::make_tuple((uint64_t)0,name("eosio.token"), name("eostothemoon"),EOS_SYMBOL)
              ).send();                         
  
              action(
                  permission_level{ name("eostothemoon"), name("active") },
                  name("eosio.token"), name("transfer"),
                  std::make_tuple(name("eostothemoon"), BOX_CONTRACT, asset(order_amount,EOS_SYMBOL), itr->BOX_swap_memo)
              ).send();

              action( 
                  permission_level{ get_self(), name("active") },
                  get_self(), name("tokenasset"),
                  std::make_tuple((uint64_t)0,itr->token_contract_address, name("eostothemoon"),itr->token_symbol)
              ).send();
                
              action(
                  permission_level{ get_self(), name("active") },
                  get_self(), name("selltoken"),
                  std::make_tuple((uint64_t)0,name("eostothemoon"), BOX_CONTRACT, itr->token_contract_address, itr->BOX_swap_memo,boxpoolid)
              ).send();

              action( 
                  permission_level{ get_self(), name("active") },
                  get_self(), name("tokenasset"),
                  std::make_tuple((uint64_t)1,name("token.defi"), name("eostothemoon"),BOX_SYMBOL)
              ).send();

              action(
                  permission_level{ get_self(), name("active") },
                  get_self(), name("selltoken"),
                  std::make_tuple((uint64_t)1,name("eostothemoon"), BOX_CONTRACT, name("token.defi"), std::string("swap,0,194"),boxpoolid)
              ).send();

             action( 
                  permission_level{ get_self(), name("active") },
                  get_self(), name("eosasset"),
                  std::make_tuple((uint64_t)1,name("eosio.token"), name("eostothemoon"),EOS_SYMBOL)
              ).send(); 

              action( 
                  permission_level{ get_self(), name("active") },
                  get_self(), name("judge"),
                  std::make_tuple()
              ).send();               
               
                    
        

    }

    [[eosio::action]]
    void dfsmine(int64_t dfspoolid,int64_t time){
      require_auth(get_self());  
      dfspool_index dfspools( get_self(), get_first_receiver().value ); 
      auto itr = dfspools.find(dfspoolid);

      check(itr!= dfspools.end(),"dfspools have not this dfspoolid" );
                  
            
              auto order_amount = (uint64_t)10000;    

              action( 
                  permission_level{ get_self(), name("active") },
                  get_self(), name("eosasset"),
                  std::make_tuple((uint64_t)0,name("eosio.token"), name("eostothemoon"),EOS_SYMBOL)
              ).send();                         
  
              action(
                  permission_level{ name("eostothemoon"), name("active") },
                  name("eosio.token"), name("transfer"),
                  std::make_tuple(name("eostothemoon"), DFS_CONTRACT, asset(order_amount,EOS_SYMBOL), itr->DFS_swap_memo)
              ).send();

              action( 
                  permission_level{ get_self(), name("active") },
                  get_self(), name("tokenasset"),
                  std::make_tuple((uint64_t)0,itr->token_contract_address, name("eostothemoon"),itr->token_symbol)
              ).send();
                
              action(
                  permission_level{ get_self(), name("active") },
                  get_self(), name("selltoken"),
                  std::make_tuple((uint64_t)0,name("eostothemoon"), DFS_CONTRACT, itr->token_contract_address, itr->DFS_swap_memo)
              ).send();

              action(
                  permission_level{ get_self(), name("active") },
                  get_self(), name("tokenasset"),
                  std::make_tuple((uint64_t)1,name("minedfstoken"), name("eostothemoon"),DFS_SYMBOL)
              ).send();

              action(
                  permission_level{ get_self(), name("active") },
                  get_self(), name("selltoken"),
                  std::make_tuple((uint64_t)1,name("eostothemoon"), DFS_CONTRACT, name("minedfstoken"), std::string("swap:39:0:8"))
              ).send();

             action( 
                  permission_level{ get_self(), name("active") },
                  get_self(), name("eosasset"),
                  std::make_tuple((uint64_t)1,name("eosio.token"), name("eostothemoon"),EOS_SYMBOL)
              ).send(); 

              action( 
                  permission_level{ get_self(), name("active") },
                  get_self(), name("judge"),
                  std::make_tuple()
              ).send();                 
                  
          
            
        
    }  



    [[eosio::action]]
    void eraseleft(uint64_t id) {
      require_auth(get_self());

      tokenleft_index tokenlefts( get_self(), get_first_receiver().value);

      auto iterator = tokenlefts.find(id);
      check(iterator != tokenlefts.end(), "Record does not exist");
      tokenlefts.erase(iterator);
    }


    [[eosio::action]]     // '["0","10,DMD","0.0000000001","eosdmdtokens","472","swap,0,472"]'
    void upsertboxpo(uint64_t id, symbol token_symbol, double token_precision,name token_contract_address, uint64_t BOX_id, std::string BOX_swap_memo) {
      require_auth( get_self() );
      boxpool_index boxpools( get_self(), get_first_receiver().value );
      auto iterator = boxpools.find(id);
      if( iterator == boxpools.end() )
      {
        boxpools.emplace(get_self(), [&]( auto& row ) {         //emplace( name payer, Lambda&& constructor ) 
        row.id = id;
        row.token_symbol = token_symbol;
        row.token_precision = token_precision;
        row.token_contract_address = token_contract_address;
        row.BOX_id = BOX_id;              
        row.BOX_swap_memo = BOX_swap_memo;     
        });
      }
      else {
        boxpools.modify(iterator, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )
        row.id = id;
        row.token_symbol = token_symbol;
        row.token_precision = token_precision;
        row.token_contract_address = token_contract_address;
        row.BOX_id = BOX_id;              
        row.BOX_swap_memo = BOX_swap_memo;
        });
      }
    }

    [[eosio::action]]
    void eraseboxpo(uint64_t id) {
      require_auth(get_self());

      boxpool_index boxpools( get_self(), get_first_receiver().value);
      auto iterator = boxpools.find(id);
      check(iterator != boxpools.end(), "Record does not exist");
      boxpools.erase(iterator);
    }

    [[eosio::action]]     // '["0","10,DMD","0.0000000001","eosdmdtokens","472","swap,0,472"]'
    void upsertdfspo(uint64_t id, symbol token_symbol, double token_precision,name token_contract_address, uint64_t DFS_id, std::string DFS_swap_memo) {
      require_auth( get_self() );
      dfspool_index dfspools( get_self(), get_first_receiver().value );
      auto iterator = dfspools.find(id);
      if( iterator == dfspools.end() )
      {
        dfspools.emplace(get_self(), [&]( auto& row ) {         //emplace( name payer, Lambda&& constructor ) 
        row.id = id;
        row.token_symbol = token_symbol;
        row.token_precision = token_precision;
        row.token_contract_address = token_contract_address;
        row.DFS_id = DFS_id;              
        row.DFS_swap_memo = DFS_swap_memo;     
        });
      }
      else {
        dfspools.modify(iterator, get_self(), [&]( auto& row ) {    //modify( const T& obj, name payer, Lambda&& updater )
        row.id = id;
        row.token_symbol = token_symbol;
        row.token_precision = token_precision;
        row.token_contract_address = token_contract_address;
        row.DFS_id = DFS_id;              
        row.DFS_swap_memo = DFS_swap_memo;
        });
      }
    }

    [[eosio::action]]
    void erasedfspo(uint64_t id) {
      require_auth(get_self());

      dfspool_index dfspools( get_self(), get_first_receiver().value);
      auto iterator = dfspools.find(id);
      check(iterator != dfspools.end(), "Record does not exist");
      dfspools.erase(iterator);
    }




    [[eosio::action]]
    void selltoken(uint64_t boxdfs_ornot,  const name& from,const name& dex_contract_address,const name& token_contract_address, const std::string& dex_swap_memo, int64_t boxpoolid ){
      require_auth(get_self());
      tokenleft_index tokenlefts(get_self(),get_first_receiver().value);
      auto token_itr = tokenlefts.find(0);
      if(boxdfs_ornot == 0){
         token_itr = tokenlefts.find(0);
         check(token_itr->balance.amount >0,"do not get Token, the balance of Token is 0. the swap id is --" + std::to_string(boxpoolid)); // + std::to_string(boxpoolid)
      }else{
         token_itr = tokenlefts.find(1);
         check(token_itr->balance.amount >0,"do not get BOX/DFS, the balance of BOX/DFS is 0. the swap id is --" + std::to_string(boxpoolid) );
      }       
      check(token_itr != tokenlefts.end(),"the table tokenlefts have no data");
      action(
          permission_level{ name("eostothemoon"), name("active") },
          token_contract_address, name("transfer"),
          std::make_tuple(from, dex_contract_address, token_itr->balance, dex_swap_memo)
      ).send();
    }

    [[eosio::action]]
    void tokenasset(uint64_t boxdfs_ornot,  const name& token_contract_account, const name& owner, const symbol& sym ){
      require_auth(get_self());
      tokenleft_index tokenlefts(get_self(),get_first_receiver().value);
      if(boxdfs_ornot == 0){
          auto token_itr = tokenlefts.find(0);      
          if( token_itr == tokenlefts.end() ){                          
              tokenlefts.emplace(get_self(), [&]( auto& row ) {
                  row.id = 0;
                  row.balance = getbalance( token_contract_account, owner, sym );                 
              });                 
          }else {
              tokenlefts.modify(token_itr, get_self(), [&]( auto& row ) {
                  row.id = 0;
                  row.balance = getbalance( token_contract_account, owner, sym );
              });
          }
      }else{
          auto token_itr = tokenlefts.find(1);      
          if( token_itr == tokenlefts.end() ){                          
              tokenlefts.emplace(get_self(), [&]( auto& row ) {
                  row.id = 1;
                  row.balance = getbalance( token_contract_account, owner, sym );                 
              });                 
          }else {
              tokenlefts.modify(token_itr, get_self(), [&]( auto& row ) {
                  row.id = 1;
                  row.balance = getbalance( token_contract_account, owner, sym );
              });
          }
      }       
    }


    [[eosio::action]]
    void eosasset(uint64_t now_after,  const name& token_contract_account, const name& owner, const symbol& sym ){
      require_auth(get_self());
      eosleft_index eoslefts(get_self(),get_first_receiver().value);
      if(now_after == 0){
          auto token_itr = eoslefts.find(0);      
          if( token_itr == eoslefts.end() ){                          
              eoslefts.emplace(get_self(), [&]( auto& row ) {
                  row.id = 0;
                  row.balance = getbalance( token_contract_account, owner, sym );                 
              });                 
          }else {
              eoslefts.modify(token_itr, get_self(), [&]( auto& row ) {
                  row.id = 0;
                  row.balance = getbalance( token_contract_account, owner, sym );
              });
          }
      }
      if(now_after == 1){
          auto token_itr = eoslefts.find(1);      
          if( token_itr == eoslefts.end() ){                          
              eoslefts.emplace(get_self(), [&]( auto& row ) {
                  row.id = 1;
                  row.balance = getbalance( token_contract_account, owner, sym );                 
              });                 
          }else {
              eoslefts.modify(token_itr, get_self(), [&]( auto& row ) {
                  row.id = 1;
                  row.balance = getbalance( token_contract_account, owner, sym );
              });
          }
      }        
    }

    [[eosio::action]]
    void judge(){
      require_auth(get_self());
      eosleft_index eoslefts(get_self(),get_first_receiver().value);
      auto eos_before = eoslefts.find(0);
      auto eos_after = eoslefts.find(1);
      check(eos_after->balance.amount > (eos_before->balance.amount + (uint64_t)1 ) ,"this is not a success mint,come on");  
      swaplog("yanghao12345"_n, "TradeMine----eosbefore = " + std::to_string(eos_before->balance.amount) + " eosafter = " + std::to_string(eos_after->balance.amount) );

    }

  private:

    void swaplog(name log_address, std::string message){
        require_auth( get_self() );
        action(
            permission_level{get_self(),"active"_n},
            get_self(),
            "notify"_n,
            std::make_tuple(log_address,message)
        ).send();
    }

    asset getbalance(  const name& token_contract_account, const name& owner, const symbol& sym){
      account_index accountstable( token_contract_account, owner.value );
      const auto& ac = accountstable.get( sym.code().raw() );
      return ac.balance;
    }



    struct [[eosio::table]] boxpool {
      uint64_t id;
      symbol token_symbol;
      double token_precision;
      name token_contract_address;
      uint64_t BOX_id; 
      std::string BOX_swap_memo;
      uint64_t primary_key() const { return id; }
    };
    using boxpool_index = eosio::multi_index<"boxpools"_n,boxpool>; 


    struct [[eosio::table]] dfspool {
      uint64_t id;
      symbol token_symbol;
      double token_precision;
      name token_contract_address;
      uint64_t DFS_id; 
      std::string DFS_swap_memo;
      uint64_t primary_key() const { return id; }
    };
    using dfspool_index = eosio::multi_index<"dfspools"_n,dfspool>; 





    
    struct [[eosio::table]] account {           
      asset balance;         
      uint64_t primary_key() const { return balance.symbol.code().raw(); }
    };
    using account_index = eosio::multi_index<"accounts"_n,account>;
   
   
    struct [[eosio::table]] tokenleft {
      uint64_t id;           
      asset balance;         
      uint64_t primary_key() const { return id; }
    };
    using tokenleft_index = eosio::multi_index<"tokenlefts"_n,tokenleft>;
   
    struct [[eosio::table]] eosleft {
      uint64_t id;           
      asset balance;               
      uint64_t primary_key() const { return id; }
    };
    using eosleft_index = eosio::multi_index<"eoslefts"_n,eosleft>;




  };



