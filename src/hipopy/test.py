from hipopy import hipo_reader

reader = hipo_reader("/Users/tylern/Data/hipo/out_clas_004203.evio.101.hipo")

reader.showInfo()
print(reader.getRecordCount())

node = reader.getNode(331, 1)  # REC::Particle pid

while(reader.next()):
    val = node.getLength()
    print(val)

assert(reader.getRecordCount() == 579), "Wrong record count!"
