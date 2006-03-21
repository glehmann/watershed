WRAP_CLASS("itk::MorphologicalWatershedImageFilter" POINTER)
  UNIQUE(label_types "${WRAP_ITK_INT};${WRAP_ITK_SIGN_INT};UL")
  WRAP_IMAGE_FILTER_COMBINATIONS("${WRAP_ITK_SCALAR}" "${label_types}")
END_WRAP_CLASS()
