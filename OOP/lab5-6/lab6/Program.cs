abstract class MusicalInstrument
{
    private string name;

    public string name_pub
    {
        get => name;
        set
        {
            if (string.IsNullOrEmpty(value))
            {
                throw new Exception("Имя не может быть пустым!");
            }
            name = value;
        }
    }
    public string manufacturer { get; set; }
    public DateTime manufactureDate { get; set; }

    protected MusicalInstrument(string name, string manufacturer, DateTime manufactureDate)
    {
        this.name = name;
        this.manufacturer = manufacturer;
        this.manufactureDate = manufactureDate;
    }
    
    abstract public void Play();
    abstract public void Tune();
}

class Guitar : MusicalInstrument
{
    public Guitar(string name, string manufacturer, DateTime manufactureDate) : base(name, manufacturer,
        manufactureDate) { }
    
    public override void Play()
    {
        Console.WriteLine($"{name_pub} гитара играет!");
    }

    public override void Tune()
    {
        Console.WriteLine($"{name_pub} гитара настроена");
    }
}

class Piano : MusicalInstrument
{
    public Piano(string name, string manufacturer, DateTime manufactureDate) : base(name, manufacturer,
        manufactureDate) { }

    public override void Play()
    {
        Console.WriteLine($"{name_pub} пианино играет");
    }

    public override void Tune()
    {
        Console.WriteLine($"{name_pub} пианино настроено");
    }
}

class Violin : MusicalInstrument
{
    public Violin(string name, string manufacturer, DateTime manufactureDate) : base(name, manufacturer,
        manufactureDate) { }

    public override void Play()
    {
        Console.WriteLine($"{name_pub} скрипка играет");
    }

    public override void Tune()
    {
        Console.WriteLine($"{name_pub} скрипка настроена");
    }
}

class Program
{
    static void Main(string[] args)
    {
        Guitar guitar = new Guitar("Гитара 1", "Производитель 1", DateTime.Today);
        guitar.Play();
        guitar.Tune();
        Piano piano = new Piano("Пианино 1", "Производитель 2",  DateTime.Today);
        piano.Play();
        piano.Tune();
        Violin violin = new Violin("Скрипка 1", "Производитель 3", DateTime.Today);
        violin.Play();
        violin.Tune();
    }
}