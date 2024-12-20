import os

from django.shortcuts import render
from django.http import HttpResponse, HttpResponseRedirect, FileResponse
from django.conf import settings
from django.urls import reverse

from .static.compresso.algorithms.files.rle import rle
from .static.compresso.algorithms.files.lzw import lzw
from .static.compresso.algorithms.files.huffman import huffman
# Create your views here.

def index(request):
    return render(request, "compresso/index.html")

def text_compress(request):
    return render(request, "compresso/textcompress.html")

def file_compress(request):
    if request.method == "POST":
        operation = "compress" if "compress" in request.POST else "decompress"
        algorithm = request.POST.get("algorithm", "")
        uploaded_file = request.FILES.get("file")

        if uploaded_file:
            upload_dir = os.path.join(settings.MEDIA_ROOT, "compresso")
            print(f"MEDIA_ROOT: {settings.MEDIA_ROOT}")
            print(f"Upload directory: {upload_dir}")        
            os.makedirs(upload_dir, exist_ok=True)

            upload_path = os.path.join(upload_dir, uploaded_file.name)
            print(f"File saved at: {upload_path}")

            with open(upload_path, "wb+") as destination:
                for chunk in uploaded_file.chunks():
                    destination.write(chunk)

            if algorithm == "RLE":
                processed_file_path = rle(upload_path, operation) 
            elif algorithm == "LZW":
                processed_file_path = lzw(upload_path, operation) 
            elif algorithm == "Huffman":
                processed_file_path = huffman(upload_path, operation) 

            if os.path.exists(processed_file_path):
                return FileResponse(open(processed_file_path, "rb"), as_attachment=True, filename=os.path.basename(processed_file_path))
                
            return HttpResponseRedirect(reverse("file"))

    else:
        return render(request, "compresso/filecompress.html")

def comparison(request):
    # Paths for the two categories
    category1_folder = os.path.join(settings.MEDIA_ROOT, "compresso", "images", "size")
    category2_folder = os.path.join(settings.MEDIA_ROOT, "compresso", "images", "time")

    # Debug: Verify paths
    if not os.path.exists(category1_folder):
        print(f"Category 1 folder not found: {category1_folder}")
    if not os.path.exists(category2_folder):
        print(f"Category 2 folder not found: {category2_folder}")

    # Generate image URLs
    category1_images = [
        f"{settings.MEDIA_URL}compresso/images/size/{img}"
        for img in os.listdir(category1_folder)
        if img.endswith((".png", ".jpg", ".jpeg"))
    ] if os.path.exists(category1_folder) else []

    category2_images = [
        f"{settings.MEDIA_URL}compresso/images/time/{img}"
        for img in os.listdir(category2_folder)
        if img.endswith((".png", ".jpg", ".jpeg"))
    ] if os.path.exists(category2_folder) else []

    # Pass the URLs to the template
    return render(request, "compresso/comparison.html", {
        "category1_images": category1_images,
        "category2_images": category2_images,
    })