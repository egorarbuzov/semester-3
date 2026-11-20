using System.Runtime.Serialization;
using System.Text.Encodings.Web;
using System.Text.Json;
using System.Xml;
using System.Xml.Serialization;

enum SerializationFormat
{
    Binary,
    Xml,
    Json
}

public class Book
{
    public string Title { get; set; } = string.Empty;
    public string Author { get; set; } = string.Empty;
    public int Year { get; set; }
    public decimal Price { get; set; }
}

class UniversalSerializer<T>
{
    public void Save(T obj, string fileName, SerializationFormat? format = null)
    {
        SerializationFormat actualFormat = format ?? GetFormat(fileName);

        if (actualFormat == SerializationFormat.Xml)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(T));
            using (FileStream stream = new FileStream(fileName, FileMode.Create))
            {
                serializer.Serialize(stream, obj);
            }
        }
        else if (actualFormat == SerializationFormat.Json)
        {
            JsonSerializerOptions options = new JsonSerializerOptions
            {
                WriteIndented = true,
                Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping
            };
            File.WriteAllText(fileName, JsonSerializer.Serialize(obj, options));
        }
        else
        {
            DataContractSerializer serializer = new DataContractSerializer(typeof(T));
            using (FileStream stream = new FileStream(fileName, FileMode.Create))
            using (XmlDictionaryWriter writer = XmlDictionaryWriter.CreateBinaryWriter(stream))
            {
                serializer.WriteObject(writer, obj);
            }
        }
    }

    public T Load(string fileName, SerializationFormat? format = null)
    {
        SerializationFormat actualFormat = format ?? GetFormat(fileName);

        if (actualFormat == SerializationFormat.Xml)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(T));
            using (FileStream stream = new FileStream(fileName, FileMode.Open))
            {
                return (T)serializer.Deserialize(stream)!;
            }
        }

        if (actualFormat == SerializationFormat.Json)
        {
            return JsonSerializer.Deserialize<T>(File.ReadAllText(fileName))!;
        }

        DataContractSerializer binarySerializer = new DataContractSerializer(typeof(T));
        using (FileStream stream = new FileStream(fileName, FileMode.Open))
        using (XmlDictionaryReader reader = XmlDictionaryReader.CreateBinaryReader(stream, XmlDictionaryReaderQuotas.Max))
        {
            return (T)binarySerializer.ReadObject(reader)!;
        }
    }

    private static SerializationFormat GetFormat(string fileName)
    {
        string extension = Path.GetExtension(fileName).ToLower();

        if (extension == ".dat")
        {
            return SerializationFormat.Binary;
        }

        if (extension == ".xml")
        {
            return SerializationFormat.Xml;
        }

        if (extension == ".json")
        {
            return SerializationFormat.Json;
        }

        throw new ArgumentException($"Неизвестное расширение файла: {extension}");
    }
}

class Program
{
    static void Main(string[] args)
    {
        Book book = new Book
        {
            Title = "Мастер и Маргарита",
            Author = "Михаил Булгаков",
            Year = 1967,
            Price = 20.00m
        };

        UniversalSerializer<Book> serializer = new UniversalSerializer<Book>();
        string[] files = { "book.dat", "book.xml", "book.json" };

        foreach (string file in files)
        {
            serializer.Save(book, file);
        }

        foreach (string file in files)
        {
            Book loadedBook = serializer.Load(file);
            Console.WriteLine($"{file}: {loadedBook.Title} — {loadedBook.Author}, {loadedBook.Year}, {loadedBook.Price:F2} ({new FileInfo(file).Length} байт)");
        }

        serializer.Save(book, "book.data", SerializationFormat.Json);
        Book forcedBook = serializer.Load("book.data", SerializationFormat.Json);
        Console.WriteLine($"Принудительный JSON в book.data: {forcedBook.Title}");

        try
        {
            serializer.Save(book, "book.txt");
        }
        catch (ArgumentException exception)
        {
            Console.WriteLine($"Ошибка: {exception.Message}");
        }
    }
}
