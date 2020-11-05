#include <iostream>
#include <vector>  
#include <string>  
#include <stdio.h>  
#include <stdlib.h> 
 
#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>  

#include <sqlite3.h>
#define EOR '"'
using namespace std;
using namespace cgicc;

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      cout<<"<p3>"<< azColName[i] << '=' << argv[i] ? argv[i] : "NULL";
      cout<<"\n</p3><br>";
   }
   printf("\n");
   return 0;
}

int main ()
{
   Cgicc formData;
   sqlite3 *db;
   string as;
   char *zErrMsg = 0;
   int  rc;
   char* sql;
   const char* data = "Callback function called";
   /*=======================================================================================*/
   cout << "Content-type:text/html\r\n\r\n";
   cout << "<meta charset="<<EOR<<"utf-8"<<EOR<<">";  //EOR因為編譯器會誤判""符號
   cout << "<html>\n";
   cout << "<head>\n";
   cout << "<title>SQL查詢網站</title>\n";
   cout << "</head>\n";
   cout << "<body>\n";
   /*===============================資料庫連結==============================================*/
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "<p1>資料庫開啟失敗: %s\n</p1>", sqlite3_errmsg(db));
   }else{
      fprintf(stdout,"<p1>資料庫成功開啟\n</p1><br>");
   }

   /*===================================接收表單=============================================*/
   form_iterator fi = formData.getElement("sql_text");   //這邊 html我用POST以標準輸出傳送資料,GET是用網址後夾帶?='變數'來傳資料
   if( !fi->isEmpty() && fi != (*formData).end()) {
      cout << "<p2>語法為：" << **fi <<"\n</p2>"<< endl;
      cout <<"<hr>";
      as=**fi;
   }else{
      cout << "No text entered for first name" << endl;
   }
   /*=================================字符轉換===============================================*/
   char * writable = new char[as.size() + 1];
   copy(as.begin(), as.end(), writable);
   writable[as.size()] = '\0';
   /*==================================執行查詢和呼叫========================================*/
   sql = writable;
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "<hr>\n");
   }
   sqlite3_close(db);
   /*=======================================================================================*/
   cout << "<br/>\n";
   cout << "</body>\n";
   cout << "</html>\n";
   
   return 0;
}
