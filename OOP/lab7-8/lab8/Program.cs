using System;

struct Book
{
    public string Title;
    public string Author;
    public int PublishYear;
    public int Pages;

    public Book(string Title, string Author, int PublishYear, int Pages)
    {
        this.Title = Title;
        this.Author = Author;
        this.PublishYear = PublishYear;
        this.Pages = Pages;
    }
    
    public int ReadTime()
    {
        return Pages * 2;
    }
}

enum Seasons
{
    Winter,
    Spring,
    Summer,
    Autumn
};

class Program
{
    static Seasons GetSeason(int month)
    {
        switch (month)
        {
            case 12:
            case 1:
            case 2:
                return Seasons.Winter;
            case 3:
            case 4:
            case 5:
                return Seasons.Spring;
            case 6:
            case 7:
            case 8:
                return Seasons.Summer;
            case 9:
            case 10:
            case 11:
                return Seasons.Autumn;
            default:
                throw new Exception("Месяц должен быть от 1 до 12");
        }
    }
    
    static void Main(string[] args)
    {
        Book[] books = new Book[5];
        books[0] = new Book("Война и мир", "Л. Н. Толстой", 1869, 1225);
        books[1] = new Book("Преступление и наказание", "Ф. М. Достоевский", 1866, 672);
        books[2] = new Book("Мастер и Маргарита", "М. А. Булгаков", 1967, 480);
        books[3] = new Book("Тихий Дон", "М. А. Шолохов", 1940, 1600);
        books[4] = new Book("Отцы и дети", "И. С. Тургенев", 1862, 288);
        
        Book thickest = books[0];
        for (int i = 1; i < books.Length; i++)
        {
            if (books[i].Pages > thickest.Pages)
            {
                thickest = books[i];
            }
        }

        Console.WriteLine($"{thickest.Title} - самая толстая");
        
        int[] months = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

        foreach (int month in months)
        {
            Seasons season = GetSeason(month);
            Console.WriteLine($"Месяц {month}: сезон — {season}");
        }
    }
}