idf_component_register(
        SRC_DIRS "."
        INCLUDE_DIRS "."
        EMBED_FILES "certs/ota_cert.pem" 
                "certs/aws_root_ca.pem" 
                "certs/certificate.pem.crt" 
                "certs/private.pem.key"
)
