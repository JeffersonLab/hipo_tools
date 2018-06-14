C-*************************************
C- Reading HIPO file from fortran
C-*************************************
      PROGRAM READ_FILE
      INTEGER NRECORDS,R,NEVENTS,E, NROWS
      INTEGER CHARGE(30), PID(30)
      REAL    PX(30),PY(30), PZ(30)
      CHARACTER SCHEMA*1024
      INTEGER   MAX_LENGTH
      INTEGER   SCHEMA_LENGTH
      INTEGER   NSCHEMAS
      INTEGER   S
C-
      CALL HIPO_OPEN_FILE(NRECORDS,'infile.hipo')
      WRITE(*,*) '[OPEN] NRECORDS = ', NRECORDS
      MAX_LENGTH = 1024
      CALL GET_DICT_LENGTH(NSCHEMAS)
      WRITE(*,*) 'NUMBER OF SCHEMAS = ',NSCHEMAS
      
      DO S=1,NSCHEMAS
         CALL READ_SCHEMA(S-1,MAX_LENGTH,SCHEMA_LENGTH,SCHEMA)
         WRITE(*,*) '-----> FORTRAN = ',SCHEMA(1:SCHEMA_LENGTH)
      ENDDO

      DO WHILE(HIPO_FILE_NEXT()>0) 

         CALL HIPO_READ_NODE_INT(331,8,NROWS,CHARGE)
         CALL HIPO_READ_NODE_INT(331,1,NROWS,PID)
         CALL HIPO_READ_NODE_FLOAT(331,2,NROWS,PX)
         IF(NROWS.GT.0) THEN
            WRITE(*,*) (PID(j),J=1,NROWS)
            WRITE(*,*) (CHARGE(j),J=1,NROWS)
            WRITE(*,*) (PX(j),J=1,NROWS)
         ENDIF
      ENDDO
      WRITE(*,*) '[DONE] done reading file'
      END
