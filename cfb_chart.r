# ==============================================================================
# 1. LOAD REQUIRED LIBRARIES AND ENSURE DEPENDENCIES ARE INSTALLED
# ==============================================================================
options(repos = c(CRAN = "https://r-project.org"))

if (!requireNamespace("cfbplotR", quietly = TRUE)) {
  message("cfbplotR not found. Installing now...")
  if (!requireNamespace("remotes", quietly = TRUE)) install.packages("remotes")
  remotes::install_github("sportsdataverse/cfbplotR", upgrade = "always")
}

invisible(lapply(c("scales", "png", "grid", "ggplot2"), function(pkg) {
  if (!requireNamespace(pkg, quietly = TRUE)) install.packages(pkg)
}))

library(cfbplotR)
library(ggplot2)
library(scales)      
library(png)         
library(grid)        

# ==============================================================================
# 2. CONFIGURE VISUAL SETTINGS AND TEXT LABELS
# ==============================================================================
team_col_name   <- "Team" 
x_axis_col      <- "Threat Value"               
y_axis_col      <- "TVD"  

plot_title    <- "2026 CFB Threat Value Analysis"
plot_subtitle <- "All P4 v P4 Games (Week 2)"
x_axis_label  <- "Threat Value"
y_axis_label  <- "Differential (Threat vs. Points)"

# --- UPDATED: CONFIGURABLE DUAL OUTPUT FILENAMES ---
output_watermarked <- "threat_chart_wm.png"
output_clean       <- "threat_chart_clean.png"

# ==============================================================================
# 3. INTERACTIVE FILE LOADING & WATERMARK CODES
# ==============================================================================
portfolio_dir <- "C:/Users/swish/OneDrive/Documents/Portfolio Practice/"

# Load dataset using standard directory path
data1 <- read.csv(paste0(portfolio_dir, "25_single_tv.csv"), check.names = FALSE)

# --- FIXED INTERACTIVE SELECTION ---
message(">>> A window has popped up! Please select watermark file...")
watermark_img <- readPNG(file.choose())

# --- APPLY FAINT/TRANSPARENT EFFECT (8% Opacity) ---
watermark_img[,,4] <- watermark_img[,,4] * 0.08

# Calculate native aspect ratio (Width / Height)
img_ratio <- ncol(watermark_img) / nrow(watermark_img)

# --- PERFECT ORIGINAL WATERMARK VIEWPORT GRAPHICS TREE ---
watermark_grob <- gTree(
  children = gList(rasterGrob(image = watermark_img, width = unit(1, "npc"), height = unit(1, "npc"))),
  vp = viewport(
    x      = unit(0.5, "npc"), 
    y      = unit(0.5, "npc"), 
    just   = "center",
    width  = unit(1, "snpc"), 
    height = unit(1/img_ratio, "snpc")
  )
)

# ==============================================================================
# 4. BUILD THE PLOT WITH TRUE BACKGROUND WALLPAPER & PSEUDO-LOG SCALING
# ==============================================================================
x_baseline <- median(data1[[x_axis_col]], na.rm = TRUE)
y_baseline <- 0  

# Base plot containing crosshairs and team components (CLEAN COPY)
base_plot <- ggplot(data1, aes(x = .data[[x_axis_col]], y = .data[[y_axis_col]])) +
  geom_vline(xintercept = x_baseline, color = "firebrick", linetype = "dashed", alpha = 0.5) +
  geom_hline(yintercept = y_baseline, color = "firebrick", linetype = "solid", linewidth = 0.7) +
  geom_point(color = "white", size = 6.5, shape = 16, alpha = 0.95) +
  geom_cfb_logos(aes(team = .data[[team_col_name]]), width = 0.05) + 
  scale_x_continuous(
    trans  = pseudo_log_trans(sigma = 5), 
    breaks = c(0, 5, seq(10, 120, by = 10))
  ) +
  scale_y_continuous(
    trans  = pseudo_log_trans(sigma = 5), 
    breaks = c(-10, -5, 0, 5, 10, 15, 20, 30, 45, 65)
  ) +
  labs(
    title    = plot_title,
    subtitle = plot_subtitle,
    x        = x_axis_label,
    y        = y_axis_label
  ) +
  theme_bw() +
  theme(
    panel.background = element_rect(fill = "transparent", color = NA),
    plot.background  = element_rect(fill = "white", color = NA),
    plot.title       = element_text(face = "bold", size = 15, hjust = 0.5, margin = margin(b=4)),
    plot.subtitle    = element_text(size = 11, hjust = 0.5, color = "gray40", margin = margin(b=10)),
    axis.title       = element_text(face = "bold", size = 11),
    axis.text.x      = element_text(size = 9, angle = 0, hjust = 0.5),
    axis.text.y      = element_text(size = 9),
    panel.grid.major = element_line(color = "gray94"),
    panel.grid.minor = element_blank() 
  )

# --- FIXED VERSION A: RENDER WATERMARK THROUGH PLOT.BACKGROUND AS A TRUE BACKGROUND CANVAS ---
my_plot_watermarked <- base_plot + 
  theme(
    panel.background = element_rect(fill = "transparent", color = NA),
    plot.background  = element_rect(
      fill  = "white", 
      color = NA
    )
  ) +
  # Force-inject our original viewport tree structure straight into the background canvas layer
  annotation_custom(
    grob = watermark_grob, 
    xmin = -Inf, xmax = Inf, 
    ymin = -Inf, ymax = Inf
  )

# Move the annotation_custom block back to base_plot structure initialization level but alter draw key
base_plot_grob <- ggplotGrob(base_plot)
watermark_grob_layer <- watermark_grob

# Alternative construction method if standard viewport still overlaps due to device mapping
my_plot_watermarked <- base_plot + 
  theme(
    panel.background = element_rect(fill = "transparent", color = NA)
  )

# Ultimate Layer Isolation Method: Append watermark directly underneath the plot layout components
my_plot_watermarked <- ggplot(data1, aes(x = .data[[x_axis_col]], y = .data[[y_axis_col]])) +
  theme_bw() +
  theme(
    panel.background = element_rect(fill = "transparent", color = NA),
    plot.background  = element_rect(fill = "white", color = NA),
    plot.title       = element_text(face = "bold", size = 15, hjust = 0.5, margin = margin(b=4)),
    plot.subtitle    = element_text(size = 11, hjust = 0.5, color = "gray40", margin = margin(b=10)),
    axis.title       = element_text(face = "bold", size = 11),
    panel.grid.major = element_line(color = "gray94"),
    panel.grid.minor = element_blank() 
  ) +
  annotation_custom(grob = watermark_grob, xmin = -Inf, xmax = Inf, ymin = -Inf, ymax = Inf) +
  geom_vline(xintercept = x_baseline, color = "firebrick", linetype = "dashed", alpha = 0.5) +
  geom_hline(yintercept = y_baseline, color = "firebrick", linetype = "solid", linewidth = 0.7) +
  geom_point(color = "white", size = 6.5, shape = 16, alpha = 0.95) +
  geom_cfb_logos(aes(team = .data[[team_col_name]]), width = 0.05) + 
  scale_x_continuous(trans = pseudo_log_trans(sigma = 5), breaks = c(0, 5, seq(10, 120, by = 10))) +
  scale_y_continuous(trans = pseudo_log_trans(sigma = 5), breaks = c(-10, -5, 0, 5, 10, 15, 20, 30, 45, 65)) +
  labs(title = plot_title, subtitle = plot_subtitle, x = x_axis_label, y = y_axis_label)


# ==============================================================================
# 5. DUAL HIGH-RESOLUTION IMAGE EXPORT (10x7 Aspect Ratio)
# ==============================================================================

# Save 1: Watermarked Copy
ggsave(
  filename = output_watermarked, 
  plot     = my_plot_watermarked, 
  width    = 10, 
  height   = 7, 
  dpi      = 300
)

# Save 2: Clean Copy (bypasses the custom grob layer decoration entirely)
ggsave(
  filename = output_clean, 
  plot     = base_plot, 
  width    = 10, 
  height   = 7, 
  dpi      = 300
)

message("Success! Both files have been generated:")
message(" -> Watermarked version: '", output_watermarked, "'")
message(" -> Clean version:       '", output_clean, "'")
