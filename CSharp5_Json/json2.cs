using System;
using System.Net;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Reflection;

namespace JSONAPI
{
    [Guid("AAA242B3-5842-4404-B6A0-D2E707DF4C9C")]
    public interface JSONCLASS
    {
        string returnwords();
        string returntest();
        string Request(string kor);
    }

    [Guid("65D5B9FE-F4FB-49B4-AB6F-1DE75D0AA750")]
    public struct JSONMessage
    {
        public PARSED message;
    }

    public class PARSED
    {
        public class INSIDE
        {
            public string srcLangType { get; set; }
            public string tarLangType { get; set; }
            public string translatedText { get; set; }
            public string engineType { get; set; }
            public string pivot { get; set; }
        }

        public string @type { get; set; }
        public string @service { get; set; }
        public string @version { get; set; }
        public INSIDE result { get; set; }
    }

    [Guid("0FC4D9D1-D013-4F34-8404-CF0817D295A3")]
    public class TRANSLATER : JSONCLASS
    {
        private static string words;
        public TRANSLATER() { }
        public TRANSLATER(string _words)
        {
            words = _words;
        }
        public string returnwords()
        { 
            return words;
        }

        public string returntest()
        {
            return "JSONTESTER2";
        }
        public string Request(string kor)
        {
            string url = "https://openapi.naver.com/v1/papago/n2mt";
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);

            request.Headers.Add("X-Naver-Client-Id", "MeXfUJRPxxCMIZwTKALb");
            request.Headers.Add("X-Naver-Client-Secret", "vexn8B3smR");
            request.Method = "POST";

            byte[] byteDataParams = Encoding.UTF8.GetBytes("source=ko&target=en&text=" + kor);
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

            var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true, };

            JSONMessage msg = JsonSerializer.Deserialize<JSONMessage>(text, options);

            //words = msg.message.result.translatedText;
            return msg.message.result.translatedText;
        }
    }
}