class Book
{
    public string Title { get; set; } = string.Empty;
    public string Author { get; set; } = string.Empty;
    public int Year { get; set; }
    public decimal Price { get; set; }
}

class Program
{
    static void Main(string[] args)
    {
        List<Book> books = new List<Book>
        {
            new Book { Title = "Мастер и Маргарита", Author = "Михаил Булгаков", Year = 1967, Price = 20.00m },
            new Book { Title = "Преступление и наказание", Author = "Федор Достоевский", Year = 1866, Price = 18.00m },
            new Book { Title = "1984", Author = "Джордж Оруэлл", Year = 1949, Price = 22.40m },
            new Book { Title = "Гарри Поттер и философский камень", Author = "Джоан Роулинг", Year = 1997, Price = 25.50m },
            new Book { Title = "Гарри Поттер и Тайная комната", Author = "Джоан Роулинг", Year = 1998, Price = 27.00m },
            new Book { Title = "Гарри Поттер и узник Азкабана", Author = "Джоан Роулинг", Year = 1999, Price = 28.90m },
            new Book { Title = "Гарри Поттер и Кубок огня", Author = "Джоан Роулинг", Year = 2001, Price = 31.20m },
            new Book { Title = "Фантастические звери и места их обитания", Author = "Джоан Роулинг", Year = 2001, Price = 15.00m },
            new Book { Title = "Гарри Поттер и Орден Феникса", Author = "Джоан Роулинг", Year = 2003, Price = 35.40m },
            new Book { Title = "Гарри Поттер и Принц-полукровка", Author = "Джоан Роулинг", Year = 2005, Price = 38.00m },
            new Book { Title = "Гарри Поттер и Дары Смерти", Author = "Джоан Роулинг", Year = 2007, Price = 42.50m },
            new Book { Title = "Сказки барда Бидля", Author = "Джоан Роулинг", Year = 2008, Price = 19.99m },
            new Book { Title = "Случайная вакансия", Author = "Джоан Роулинг", Year = 2012, Price = 24.30m }
        };

        string targetAuthor = "Джоан Роулинг";

        Console.WriteLine($"Книги автора {targetAuthor}, изданные после 2000 года:");
        Console.WriteLine("(отсортированы по убыванию цены)");

        var authorBooks = from book in books
            where book.Author == targetAuthor && book.Year > 2000
            orderby book.Price descending
            select book;
        

        foreach (Book book in authorBooks)
        {
            Console.WriteLine($"  {book.Title} ({book.Year}) — {book.Price:F2}");
        }

        Console.WriteLine();
        Console.WriteLine("Книги, сгруппированные по году издания:");

        var booksByYear = books
            .GroupBy(book => book.Year)
            .OrderBy(group => group.Key)
            .Select(group => new
            {
                Year = group.Key,
                Count = group.Count(),
                AveragePrice = group.Average(book => book.Price)
            });

        foreach (var group in booksByYear)
        {
            Console.WriteLine($"  {group.Year}: книг — {group.Count}, средняя цена — {group.AveragePrice:F2}");
        }
    }
}
