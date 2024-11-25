from django.urls import path
from django.conf import settings
from django.conf.urls.static import static

from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("text", views.text_compress, name="text"),
    path("file", views.file_compress, name="file")
]

urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)