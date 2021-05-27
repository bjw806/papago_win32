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
    [Guid("D664B01C-7113-4C93-9860-F9FEFAB8E4BF")]
    public interface JSONCLASS
    {
        string returnwords();
        string returntest();
        string Request(string kor);
    }

    [Guid("1F54E837-77BF-47B0-8420-FC2ACD1E8F9F")]
    public struct JSONMessage
    {
        [JsonInclude] public Message message;
    }

    [Guid("531440FA-2364-4743-B9BD-9A8493EA4BC1")]
    public class Message
    {
        public class Result
        {
            [JsonInclude] public string srcLangType { get; set; }
            [JsonInclude] public string tarLangType { get; set; }
            [JsonInclude] public string translatedText { get; set; }
            [JsonInclude] public string engineType { get; set; }
            [JsonInclude] public string pivot { get; set; }
        }

        [JsonInclude] public string @type { get; set; }
        [JsonInclude] public string @service { get; set; }
        [JsonInclude] public string @version { get; set; }
        [JsonInclude] public Result result { get; set; }
    }

    [Guid("5B3293E3-4A21-4510-A49D-6B4B00D41CC4")]
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
            return "returnTEST";
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

            words = msg.message.result.translatedText;
            return msg.message.result.translatedText;
        }
    }
}