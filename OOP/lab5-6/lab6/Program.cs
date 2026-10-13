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
                throw new Exception("Имя должно быть задано");
            }
            name = value;
        }
    }
    private string manufacturer { get; set; }
    private DateTime manufactureDate { get; set; }

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
        Console.WriteLine($"{name_pub}: дзынь-дзынь! Перебор по струнам.");
    }

    public override void Tune()
    {
        Console.WriteLine($"{name_pub}: подтягиваем колки, проверяем строй.");
    }
}

class Piano : MusicalInstrument
{
    public Piano(string name, string manufacturer, DateTime manufactureDate) : base(name, manufacturer,
        manufactureDate) { }

    public override void Play()
    {
        Console.WriteLine($"{name_pub}: плим-плим! Играем аккордами.");
    }

    public override void Tune()
    {
        Console.WriteLine($"{name_pub}: мастер настраивает по камертону.");
    }
}

class Violin : MusicalInstrument
{
    public Violin(string name, string manufacturer, DateTime manufactureDate) : base(name, manufacturer,
        manufactureDate) { }

    public override void Play()
    {
        Console.WriteLine($"{name_pub}: и-и-иу! Играем смычком с вибрато.");
    }

    public override void Tune()
    {
        Console.WriteLine($"{name_pub}: настраиваем по квинтам.");
    }
}

class Program
{
    static void Main(string[] args)
    {
        MusicalInstrument[] orchestra =
        {
            new Guitar("Fender", "Fender", new DateTime(2020, 5, 1)),
            new Piano("Yamaha", "Yamaha", new DateTime(2018, 9, 15)),
            new Violin("Страдивари", "Страдивари", new DateTime(2015, 3, 10))
        };
        
        foreach (var instr in orchestra)
        {
            instr.Play();
        }

        foreach (var instr in orchestra)
        {
            instr.Tune();
        }

        try { orchestra[0].name_pub = ""; }
        catch (Exception ex) { Console.WriteLine($"Ошибка: {ex.Message}"); }

    }
}