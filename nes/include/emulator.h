class emulator {
public:
    emulator(int memory_size, int eip, int esp);
    void run();
private:
    void execute();
};
