using System;
using System.Net;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace PAPAGOAPI
{

    [Guid("DCE2E397-9CCD-47B9-BEBA-5314C139166B")]
    public interface ICallClass{
        void setlangCODE(string _lang);
        string getlangJSON();
        string getlangCODE();
        void consoleprint();
        string returnwords();
        string returntest();
        string parseTT(string str);
        string parseLT(string str);
        int lengthof(string str);
        void setclipboard(string str);
        string readfile(string str);
        string connect(string str, string str2, int i);
        string translate(string str, string source, string target);
        string language(string str);
        }

    [Guid("B319AA4F-8855-4BAC-93E0-138A275DBE52")]
    public class APITranslate : ICallClass
    {
        private static string words;
        private static string langJSON;
        private static string langCODE;
        public APITranslate() { }
        public APITranslate(string _words){
            words = _words;
        }
        public void setlangCODE(string _lang){
            langCODE = _lang;
        }
        public string getlangJSON(){
            return langJSON;
        }
        public string getlangCODE(){
            return langCODE;
        }

        public void consoleprint(){
            Console.WriteLine(words);
        }

        public string returnwords(){
            return words;
        }

        public string returntest(){
            return "returnTEST";
        }

        public string parseTT(string str){
            int pos1 = str.IndexOf("\"translatedText\":") + 18;
            //int pos2 = str.IndexOf("\",\"engineType\":");//무료
            int pos2 = str.IndexOf("\"}}}");//유료
            string parsed = str.Substring(pos1, pos2 - pos1);
            return parsed;
        } 
        public string parseLT(string str){
            int pos1 = str.IndexOf("\"langCode\":\"") + 12;
            int pos2 = str.IndexOf("}") - 1;
            string parsed = str.Substring(pos1, pos2 - pos1);
            return parsed;
        }
        public int lengthof(string str){
            return str.Length;
        }
        public void setclipboard(string str){
            Clipboard.SetText(str);
        }
        public string readfile(string str){
            return System.IO.File.ReadAllText(str);
        }
        public string connect(string str, string str2, int i){
            if (i == 0)
                return (str + str2);
            else 
                return (str + "\r\n" + str2);
        }
        public string translate(string str, string source, string target){
            if (str == "\r")
                return "";
            string url = "https://naveropenapi.apigw.ntruss.com/nmt/v1/translation";
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            request.Headers.Add("X-NCP-APIGW-API-KEY-ID", "lovlbnemmp");
            request.Headers.Add("X-NCP-APIGW-API-KEY", "nccC8Z1vdCW0emVimVgH5Vv9b6UYgzJ6NGJd0PaW");
            request.Method = "POST";
            string query = str;
            // 개행문자 삭제
            //query = query.Replace("\n", "");
            //query = query.Replace("\r", " ");

            //byte[] byteDataParams = Encoding.UTF8.GetBytes("source=ko&target=en&text=" + query);
            byte[] byteDataParams = Encoding.UTF8.GetBytes("source=" + source + "&target=" + target + "&text=" + query);
            request.ContentType = "application/x-www-form-urlencoded";
            request.ContentLength = byteDataParams.Length;
            Stream st = request.GetRequestStream();
            st.Write(byteDataParams, 0, byteDataParams.Length);
            st.Close();
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream stream = response.GetResponseStream();
            StreamReader reader = new StreamReader(stream, Encoding.UTF8);
            string text = reader.ReadToEnd();
            stream.Close();
            response.Close();
            reader.Close();
            //Console.WriteLine(text);
            words = parseTT(text);
            words = words.Replace("\\\"", "\"");
            return words;
        }

        public string language(string str){
            string url = "https://naveropenapi.apigw.ntruss.com/langs/v1/dect";
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            request.Headers.Add("X-NCP-APIGW-API-KEY-ID", "lovlbnemmp");
            request.Headers.Add("X-NCP-APIGW-API-KEY", "nccC8Z1vdCW0emVimVgH5Vv9b6UYgzJ6NGJd0PaW");
            request.Method = "POST";
            string query = str;
            //"오늘 날씨는 어떻습니까?";
            byte[] byteDataParams = Encoding.UTF8.GetBytes("query=" + query);
            request.ContentType = "application/x-www-form-urlencoded";
            request.ContentLength = byteDataParams.Length;
            Stream st = request.GetRequestStream();
            st.Write(byteDataParams, 0, byteDataParams.Length);
            st.Close();
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream stream = response.GetResponseStream();
            StreamReader reader = new StreamReader(stream, Encoding.UTF8);
            string text = reader.ReadToEnd();
            stream.Close();
            response.Close();
            reader.Close();
            //Console.WriteLine(text);
            langJSON = text;
            return parseLT(text);
        }
    }
}