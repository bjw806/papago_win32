using System;
using System.Net;
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

namespace JSONPARSER
{
    [Guid("9684977E-48AD-4823-8B5C-921170161E96")]
    public interface JSONPARSERCLASS
    {
        //string jsonconvert(string json);
        string returntest();
    }

    [Guid("F354218D-A19C-4F5C-8227-A938A7F2B54B")]
    public class JSONPARSE : JSONPARSERCLASS
    {
        private static string words;
        public class Product {
            public class Output
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
            public Output result { get; set; }
            //public List<string> Sizes { get; set; }
        }

        /*public string jsonconvert(string json)
        {
            string json2 = @"[
                {
                    'message': {
                         '@type': 'response',
                         '@service': 'naverservice.nmt.proxy',
                         '@version': '1.0.0',
                         'result': {
                               'srcLangType':'ko',
                               'tarLangType':'en',
                               'translatedText': 'tea'
                }
            }]";

            //List<Product> products = JsonConvert.DeserializeObject<List<Product>>(json);

            // Serialize (Object -> JsonString)

            //Console.WriteLine(products.Count); //2

            //Product p1 = products[0];
            //string p1 = products[0];
            //Console.WriteLine(p1.Name); //Product 1
            //return p1.result.translatedText;
        }*/
        public string returntest()
        {
            return "returnTEST";
        }
       
    }
}
