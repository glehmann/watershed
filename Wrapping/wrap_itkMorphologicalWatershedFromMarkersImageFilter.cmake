WRAP_CLASS("itk::MorphologicalWatershedFromMarkersImageFilter" POINTER)
  FOREACH(d ${WRAP_DIMS})
    COND_WRAP("${ITKM_ID${d}}${ITKM_IUL${d}}" "${ITKT_ID${d}},${ITKT_IUL${d}}" "D") # needed for watershed
    COND_WRAP("${ITKM_ID${d}}${ITKM_IUS${d}}" "${ITKT_ID${d}},${ITKT_IUS${d}}" "D;US")
    COND_WRAP("${ITKM_ID${d}}${ITKM_IUC${d}}" "${ITKT_ID${d}},${ITKT_IUC${d}}" "D;UC")
    COND_WRAP("${ITKM_ID${d}}${ITKM_ISL${d}}" "${ITKT_ID${d}},${ITKT_ISL${d}}" "D;SL")
    COND_WRAP("${ITKM_ID${d}}${ITKM_ISS${d}}" "${ITKT_ID${d}},${ITKT_ISS${d}}" "D;SS")
    COND_WRAP("${ITKM_ID${d}}${ITKM_ISC${d}}" "${ITKT_ID${d}},${ITKT_ISC${d}}" "D;SC")
    
    COND_WRAP("${ITKM_IF${d}}${ITKM_IUL${d}}" "${ITKT_IF${d}},${ITKT_IUL${d}}" "F") # needed for watershed
    COND_WRAP("${ITKM_IF${d}}${ITKM_IUS${d}}" "${ITKT_IF${d}},${ITKT_IUS${d}}" "F;US")
    COND_WRAP("${ITKM_IF${d}}${ITKM_IUC${d}}" "${ITKT_IF${d}},${ITKT_IUC${d}}" "F;UC")
    COND_WRAP("${ITKM_IF${d}}${ITKM_ISL${d}}" "${ITKT_IF${d}},${ITKT_ISL${d}}" "F;SL")
    COND_WRAP("${ITKM_IF${d}}${ITKM_ISS${d}}" "${ITKT_IF${d}},${ITKT_ISS${d}}" "F;SS")
    COND_WRAP("${ITKM_IF${d}}${ITKM_ISC${d}}" "${ITKT_IF${d}},${ITKT_ISC${d}}" "F;SC")
    
    COND_WRAP("${ITKM_IUL${d}}${ITKM_IUL${d}}" "${ITKT_IUL${d}},${ITKT_IUL${d}}" "UL")
    COND_WRAP("${ITKM_IUL${d}}${ITKM_IUS${d}}" "${ITKT_IUL${d}},${ITKT_IUS${d}}" "UL;US")
    COND_WRAP("${ITKM_IUL${d}}${ITKM_IUC${d}}" "${ITKT_IUL${d}},${ITKT_IUC${d}}" "UL;UC")
    COND_WRAP("${ITKM_IUL${d}}${ITKM_ISL${d}}" "${ITKT_IUL${d}},${ITKT_ISL${d}}" "UL;SL")
    COND_WRAP("${ITKM_IUL${d}}${ITKM_ISS${d}}" "${ITKT_IUL${d}},${ITKT_ISS${d}}" "UL;SS")
    COND_WRAP("${ITKM_IUL${d}}${ITKM_ISC${d}}" "${ITKT_IUL${d}},${ITKT_ISC${d}}" "UL;SC")
    
    COND_WRAP("${ITKM_IUS${d}}${ITKM_IUL${d}}" "${ITKT_IUS${d}},${ITKT_IUL${d}}" "US") # needed for watershed
    COND_WRAP("${ITKM_IUS${d}}${ITKM_IUS${d}}" "${ITKT_IUS${d}},${ITKT_IUS${d}}" "US")
    COND_WRAP("${ITKM_IUS${d}}${ITKM_IUC${d}}" "${ITKT_IUS${d}},${ITKT_IUC${d}}" "US;UC")
    COND_WRAP("${ITKM_IUS${d}}${ITKM_ISL${d}}" "${ITKT_IUS${d}},${ITKT_ISL${d}}" "US;SL")
    COND_WRAP("${ITKM_IUS${d}}${ITKM_ISS${d}}" "${ITKT_IUS${d}},${ITKT_ISS${d}}" "US;SS")
    COND_WRAP("${ITKM_IUS${d}}${ITKM_ISC${d}}" "${ITKT_IUS${d}},${ITKT_ISC${d}}" "US;SC")
    
    COND_WRAP("${ITKM_IUC${d}}${ITKM_IUL${d}}" "${ITKT_IUC${d}},${ITKT_IUL${d}}" "UC") # needed for watershed
    COND_WRAP("${ITKM_IUC${d}}${ITKM_IUS${d}}" "${ITKT_IUC${d}},${ITKT_IUS${d}}" "UC;US")
    COND_WRAP("${ITKM_IUC${d}}${ITKM_IUC${d}}" "${ITKT_IUC${d}},${ITKT_IUC${d}}" "UC")
    COND_WRAP("${ITKM_IUC${d}}${ITKM_ISL${d}}" "${ITKT_IUC${d}},${ITKT_ISL${d}}" "UC;SL")
    COND_WRAP("${ITKM_IUC${d}}${ITKM_ISS${d}}" "${ITKT_IUC${d}},${ITKT_ISS${d}}" "UC;SS")
    COND_WRAP("${ITKM_IUC${d}}${ITKM_ISC${d}}" "${ITKT_IUC${d}},${ITKT_ISC${d}}" "UC;SC")
    
    COND_WRAP("${ITKM_ISL${d}}${ITKM_IUL${d}}" "${ITKT_ISL${d}},${ITKT_IUL${d}}" "SL") # needed for watershed
    COND_WRAP("${ITKM_ISL${d}}${ITKM_IUS${d}}" "${ITKT_ISL${d}},${ITKT_IUS${d}}" "SL;US")
    COND_WRAP("${ITKM_ISL${d}}${ITKM_IUC${d}}" "${ITKT_ISL${d}},${ITKT_IUC${d}}" "SL;UC")
    COND_WRAP("${ITKM_ISL${d}}${ITKM_ISL${d}}" "${ITKT_ISL${d}},${ITKT_ISL${d}}" "SL")
    COND_WRAP("${ITKM_ISL${d}}${ITKM_ISS${d}}" "${ITKT_ISL${d}},${ITKT_ISS${d}}" "SL;SS")
    COND_WRAP("${ITKM_ISL${d}}${ITKM_ISC${d}}" "${ITKT_ISL${d}},${ITKT_ISC${d}}" "SL;SC")
    
    COND_WRAP("${ITKM_ISS${d}}${ITKM_IUL${d}}" "${ITKT_ISS${d}},${ITKT_IUL${d}}" "SS") # needed for watershed
    COND_WRAP("${ITKM_ISS${d}}${ITKM_IUS${d}}" "${ITKT_ISS${d}},${ITKT_IUS${d}}" "SS;US")
    COND_WRAP("${ITKM_ISS${d}}${ITKM_IUC${d}}" "${ITKT_ISS${d}},${ITKT_IUC${d}}" "SS;UC")
    COND_WRAP("${ITKM_ISS${d}}${ITKM_ISL${d}}" "${ITKT_ISS${d}},${ITKT_ISL${d}}" "SS;SL")
    COND_WRAP("${ITKM_ISS${d}}${ITKM_ISS${d}}" "${ITKT_ISS${d}},${ITKT_ISS${d}}" "SS")
    COND_WRAP("${ITKM_ISS${d}}${ITKM_ISC${d}}" "${ITKT_ISS${d}},${ITKT_ISC${d}}" "SS;SC")
    
    COND_WRAP("${ITKM_ISC${d}}${ITKM_IUL${d}}" "${ITKT_ISC${d}},${ITKT_IUL${d}}" "SC") # needed for watershed
    COND_WRAP("${ITKM_ISC${d}}${ITKM_IUS${d}}" "${ITKT_ISC${d}},${ITKT_IUS${d}}" "SC;US")
    COND_WRAP("${ITKM_ISC${d}}${ITKM_IUC${d}}" "${ITKT_ISC${d}},${ITKT_IUC${d}}" "SC;UC")
    COND_WRAP("${ITKM_ISC${d}}${ITKM_ISL${d}}" "${ITKT_ISC${d}},${ITKT_ISL${d}}" "SC;SL")
    COND_WRAP("${ITKM_ISC${d}}${ITKM_ISS${d}}" "${ITKT_ISC${d}},${ITKT_ISS${d}}" "SC;SS")
    COND_WRAP("${ITKM_ISC${d}}${ITKM_ISC${d}}" "${ITKT_ISC${d}},${ITKT_ISC${d}}" "SC")
  ENDFOREACH(d)

END_WRAP_CLASS()
