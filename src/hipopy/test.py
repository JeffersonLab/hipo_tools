from hipopy import hipo_reader

reader = hipo_reader("/Users/tylern/Data/hipo/out_clas_004203.evio.101.hipo")

rec_part_pid = reader.getNode(331, 1)  # REC::Particle pid

num = 0
while(reader.next() and num < 50):
    for i in range(0, rec_part_pid.getLength()):
        print(rec_part_pid.getValue(i), end='  ')
    num += 1
    print()
