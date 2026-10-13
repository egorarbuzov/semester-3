enum ShipStatus
{
    Normal,
    Damaged,
    Critical,
    Destroyed
}

enum CrewRole
{
    Captain,
    Pilot,
    Engineer,
    Medic,
    Scientist
}

enum HealthStatus
{
    Healthy,
    Injured,
    Critical,
    Dead
}

struct CrewMember
{
    public string Name;
    public CrewRole Role;
    public int Skill;
    public HealthStatus Health;

    public CrewMember(string name, CrewRole role, int skill, HealthStatus health)
    {
        if (skill < 1 || skill > 10)
        {
            throw new Exception("Навык должен быть от 1 до 10");
        }
        else
        {
            this.Skill = skill;
        }

        this.Name = name;
        this.Role = role;
        this.Health = health;
    }
}

struct Spaceship
{
    public string Name;
    public decimal Speed;
    public decimal Fuel;
    public CrewMember[] Crew;
    public ShipStatus Status;

    public Spaceship(string name, decimal speed, decimal fuel, CrewMember[] crew)
    {
        this.Name = name;
        this.Speed = speed;
        this.Fuel = fuel;
        this.Crew = crew;
        this.Status = ShipStatus.Normal;
    }

    public decimal GetRange()
    {
        if (this.Status == ShipStatus.Destroyed)
        {
            return 0;
        }
        else if (this.Status == ShipStatus.Critical)
        {
            return this.Fuel * this.Speed / 2;
        }
        else if (this.Status == ShipStatus.Damaged)
        {
            return this.Fuel * this.Speed / 1.5m;
        }
        else
        {
            return this.Fuel * this.Speed;
        }
    }

    public bool CanDoMission()
    {
        if (this.Status == ShipStatus.Destroyed || this.Status == ShipStatus.Critical)
        {
            return false;
        }

        bool hasCaptain = false;
        bool hasPilot = false;
        bool hasEngineer = false;

        for (int i = 0; i < this.Crew.Length; i++)
        {
            if (this.Crew[i].Health == HealthStatus.Dead)
            {
                continue;
            }

            if (this.Crew[i].Role == CrewRole.Captain)
            {
                hasCaptain = true;
            }
            else if (this.Crew[i].Role == CrewRole.Pilot)
            {
                hasPilot = true;
            }
            else if (this.Crew[i].Role == CrewRole.Engineer)
            {
                hasEngineer = true;
            }
        }

        if (hasCaptain && hasPilot && hasEngineer)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public void HitMeteorite(int damage)
    {
        if (damage <= 0)
        {
            throw new Exception("Урон должен быть больше 0");
        }

        if (this.Status == ShipStatus.Normal)
        {
            this.Status = ShipStatus.Damaged;
        }
        else if (this.Status == ShipStatus.Damaged)
        {
            this.Status = ShipStatus.Critical;
        }
        else if (this.Status == ShipStatus.Critical)
        {
            this.Status = ShipStatus.Destroyed;
        }

        Console.WriteLine($"[{this.Name}] Попадание метеорита! Урон: {damage}. Состояние: {this.Status}");
    }

    public void Repair()
    {
        if (this.Status == ShipStatus.Destroyed)
        {
            throw new Exception("Уничтоженный корабль нельзя починить");
        }

        if (this.Status == ShipStatus.Critical)
        {
            this.Status = ShipStatus.Damaged;
        }
        else if (this.Status == ShipStatus.Damaged)
        {
            this.Status = ShipStatus.Normal;
        }
        else
        {
            Console.WriteLine($"[{this.Name}] Корабль и так в порядке");
            return;
        }

        Console.WriteLine($"[{this.Name}] Ремонт выполнен. Состояние: {this.Status}");
    }

    public void HealCrew()
    {
        for (int i = 0; i < this.Crew.Length; i++)
        {
            if (this.Crew[i].Health == HealthStatus.Injured)
            {
                this.Crew[i].Health = HealthStatus.Healthy;
                Console.WriteLine($"[{this.Name}] {this.Crew[i].Name} вылечен");
            }
            else if (this.Crew[i].Health == HealthStatus.Critical)
            {
                this.Crew[i].Health = HealthStatus.Injured;
                Console.WriteLine($"[{this.Name}] {this.Crew[i].Name} стабилизирован");
            }
        }
    }

    public void ShowCrew()
    {
        Console.WriteLine($"--- Экипаж [{this.Name}] ---");
        for (int i = 0; i < this.Crew.Length; i++)
        {
            Console.WriteLine($"{this.Crew[i].Name} | {this.Crew[i].Role} | навык {this.Crew[i].Skill} | {this.Crew[i].Health}");
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        CrewMember[] crew = new CrewMember[4];
        crew[0] = new CrewMember("Иван", CrewRole.Captain, 9, HealthStatus.Healthy);
        crew[1] = new CrewMember("Пётр", CrewRole.Pilot, 8, HealthStatus.Healthy);
        crew[2] = new CrewMember("Анна", CrewRole.Engineer, 7, HealthStatus.Healthy);
        crew[3] = new CrewMember("Олег", CrewRole.Medic, 6, HealthStatus.Healthy);

        Spaceship ship = new Spaceship("Марс-1", 100, 500, crew);

        Console.WriteLine($"Корабль: {ship.Name}");
        Console.WriteLine($"Дальность полёта: {ship.GetRange()}");
        Console.WriteLine($"Миссия возможна: {ship.CanDoMission()}");

        ship.ShowCrew();

        Console.WriteLine("--- Летим к Марсу ---");

        ship.HitMeteorite(30);
        Console.WriteLine($"Дальность после повреждения: {ship.GetRange()}");
        Console.WriteLine($"Миссия возможна: {ship.CanDoMission()}");

        crew[2].Health = HealthStatus.Injured;
        ship.Crew = crew;
        ship.ShowCrew();

        ship.Repair();
        ship.HealCrew();

        Console.WriteLine($"Дальность после ремонта: {ship.GetRange()}");
        Console.WriteLine($"Миссия возможна: {ship.CanDoMission()}");

        ship.ShowCrew();
    }
}