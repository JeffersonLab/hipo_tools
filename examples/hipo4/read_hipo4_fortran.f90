!-*************************************
!- Reading HIPO file from fortran
!-*************************************
PROGRAM READ_HIPO4_FILE
  IMPLICIT NONE
  INTEGER i
  REAL PX(32),PY(32),PZ(32)
  INTEGER nevents , PID(32), rec_Particle, rec_Calorimeter, len_rec_Particle
  INTEGER,EXTERNAL :: READER_NEXT, GET_INT, GET_FLOAT, ADD_SCHEMA, GET_ROWS
  CALL HIPO_OPEN_FILE(nevents,'infile.hipo')

  CALL SHOW_DICT()
  rec_Particle = ADD_SCHEMA('REC::Particle')
  rec_Calorimeter = ADD_SCHEMA('REC::Calorimeter')

  WRITE (*,*) '[OPEN] NEVENTS = ' , nevents

  DO WHILE(READER_NEXT().EQ.1)
     len_rec_Particle = GET_ROWS(rec_particle)
     CALL GET_INT_NODE(rec_particle, 0, PID)
     CALL GET_FLOAT_NODE(rec_particle, 1, PX)
     CALL GET_FLOAT_NODE(rec_particle, 2, PY)
     CALL GET_FLOAT_NODE(rec_particle, 3, PZ)
     DO i=1, len_rec_Particle
        WRITE(*,*) i, PID(i), PX(i), PY(i), PZ(i)
     ENDDO
  ENDDO
  WRITE (*,*) '[DONE] done reading file'


END PROGRAM READ_HIPO4_FILE
